#include "Network/Client.hxx"
#include "stdafx.hxx"

void Client::connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    std::lock_guard<std::mutex> lock(mutex);
    socket.setBlocking(true); // Blocking mode for connection
    setReady(false);

    pktBuf.clear();
    pktBuf << "ASK";

    if (!send(pktBuf, ip, port))
    {
        logger.logError("Could not connect to " + ip.toString() + ":" + std::to_string(port) + "");
        setReady(true);
        setConnected(false);
        return;
    }

    if (socketSelector.wait(sf::seconds(timeout)))
    {
        if (socketSelector.isReady(socket))
        {
            if (socket.receive(pktBuf, ipBuffer, portBuf) == sf::Socket::Status::Done)
            {
                setReady(true);
                std::string header;
                pktBuf >> header;

                if (header == "ACK+UID")
                {
                    pktBuf >> myUid;
                    handleServerACKUID(ipBuffer.value(), portBuf);
                }
                else if (header == "RFS")
                {
                    handleServerRFS(ipBuffer.value(), portBuf);
                }
                else
                {
                    handleServerBadResponse(ipBuffer.value(), portBuf);
                }
            }
        }
    }
    else
    {
        logger.logError("Connection timeout: " + ip.toString() + ":" + std::to_string(port), false);
        setReady(true);
        setConnected(false);
    }

    socket.setBlocking(false);
}

void Client::listenerThread()
{
    while (connected)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (socketSelector.wait(sf::seconds(10.f)) && socketSelector.isReady(socket))
        {
            pktBuf.clear();
            std::optional<sf::IpAddress> ip;

            if (socket.receive(pktBuf, ip, portBuf) == sf::Socket::Status::Done && ip)
            {
                if (ip.value() != serverIp || portBuf != serverPort)
                    continue;

                std::pair<PacketAddress, sf::Packet> packet({ip.value(), portBuf}, sf::Packet(pktBuf));
                packetQueue.push(packet);

                handler();
            }
        }
    }
}

void Client::handler()
{
    while (connected)
    {
        std::lock_guard<std::mutex> lock(mutex);

        for (auto opt = consumePacket(); opt.has_value(); opt = consumePacket())
        {
            auto &[pkt_addr, pkt] = opt.value();

            std::string header;
            pkt >> header;

            if (header == "KIL")
            {
                disconnect();
                break;
            }
            else if (header == "FILE")
            {
                receiveFile("Assets/Client/");
            }
        }
    }
}

void Client::handleServerACKUID(const sf::IpAddress &ip, const unsigned short &port)
{
    logger.logInfo("Connected to server: " + ip.toString() + ":" + std::to_string(port) + "");
    serverIp = ip;
    serverPort = port;

    pktBuf.clear();
    pktBuf << "UID+ACK" << myUid;

    send(pktBuf);
    setReady(true);
    setConnected(true);

    std::thread(&Client::listenerThread, this).detach();
}

void Client::handleServerRFS(const sf::IpAddress &ip, const unsigned short &port)
{
    logger.logError("Connection refused by server " + ip.toString() + ":" + std::to_string(port) + ".", false);
    setConnected(false);
}

void Client::handleServerBadResponse(const sf::IpAddress &ip, const unsigned short &port)
{
    logger.logError("Bad response from server " + ip.toString() + ":" + std::to_string(port) + ".", false);
    setConnected(false);
}

void Client::setConnected(const bool &connected)
{
    this->connected = connected;
}

void Client::setReady(const bool &ready)
{
    this->ready = ready;
}

Client::Client() : logger("Client"), serverIp(0, 0, 0, 0), serverPort(0), myUid(0), ready(true), connected(false)
{
    socket.setBlocking(false);

    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
        logger.logError("Could not bind to a port");

    socketSelector.add(socket);
}

Client::~Client()
{
}

void Client::connect(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    if (connected)
        logger.logError("Already connected to " + serverIp.toString() + ":" + std::to_string(serverPort), false);
    else
        std::thread(&Client::connectorThread, this, ip, port, timeout).detach();
}

void Client::disconnect()
{
    if (!connected)
    {
        logger.logError("Not connected to any server.", false);
        return;
    }

    pktBuf.clear();
    pktBuf << "KIL";

    if (!send(pktBuf))
        logger.logError("Failed to communicate with server. Disconnecting anyway.");

    setConnected(false);
    logger.logInfo("Disconnected from server " + serverIp.toString() + ":" + std::to_string(serverPort) + ".");

    serverIp = sf::IpAddress(0, 0, 0, 0);
    serverPort = 0;
}

const bool Client::isReady() const
{
    return ready;
}

const bool Client::isConnected() const
{
    return connected;
}

const bool Client::send(sf::Packet &packet)
{
    if (socket.send(packet, serverIp, serverPort) != sf::Socket::Status::Done)
    {
        logger.logError("Error sending packet to " + serverIp.toString() + ":" + std::to_string(serverPort), false);
        return false;
    }
    packet.clear();
    return true;
}

const bool Client::send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port)
{
    if (socket.send(packet, ip, port) != sf::Socket::Status::Done)
    {
        logger.logError("Error sending packet to " + ip.toString() + ":" + std::to_string(port), false);

        return false;
    }
    return true;
}

void Client::sendFile(const std::filesystem::path &path, std::ios::openmode &mode)
{
    if (!isConnected())
        logger.logError("Not connected to any server.");

    if (!File::validatePath(path))
        logger.logError("File \"" + path.string() + "\" does not exist.");

    std::ifstream file(path, mode);
    if (!file.is_open())
        logger.logError("Could not open file \"" + path.string() + "\"");

    File::FileDescriptor fd = File::createFileDescriptor(path, mode);

    if (fd.filesize > sf::UdpSocket::MaxDatagramSize)
        logger.logError("File \"" + path.string() + "\" too large (" + std::to_string(fd.filesize) + " bytes).");

    pktBuf.clear();
    pktBuf << "FILE" << myUid << fd;

    char c;
    while (file.get(c))
        pktBuf << c;

    file.close();

    if (send(pktBuf))
        logger.logInfo("Sent file \"" + path.string() + "\" (" + std::to_string(fd.filesize) + " bytes) to " +
                       serverIp.toString());
}

void Client::receiveFile(const std::filesystem::path &folder)
{
    if (!File::validatePath(folder))
        std::filesystem::create_directory(folder);

    File::FileDescriptor fd;
    pktBuf >> fd;

    std::ofstream file(folder / fd.filename, static_cast<std::ios::openmode>(fd.mode));
    if (!file.is_open())
        logger.logError("Could not write file \"" + (folder / fd.filename).string() + "\"");

    std::uint8_t c;
    while (pktBuf >> c)
        file.put(c);

    file.close();
    logger.logInfo("Received file \"" + (folder / fd.filename).string() + "\" (" + std::to_string(fd.filesize) +
                   " bytes).");
}

std::optional<std::pair<PacketAddress, sf::Packet>> Client::consumePacket()
{
    if (packetQueue.empty())
        return std::nullopt;

    auto packet = packetQueue.front();
    packetQueue.pop();
    return packet;
}