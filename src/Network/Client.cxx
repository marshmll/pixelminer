#include "Network/Client.hxx"
#include "stdafx.hxx"

void Client::connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    std::lock_guard<std::mutex> lock(mutex);
    socket.setBlocking(true); // Blocking mode for connection
    setReady(false);

    sf::Packet pktBuf;

    pktBuf << "ASK+UUID" << myUuid;

    if (!send(pktBuf, ip, port))
    {
        logger.logError(_("Could not connect to ") + ip.toString() + ":" + std::to_string(port) + "");
        setReady(true);
        setStatus(ClientStatus::SockError);
        socket.setBlocking(false);
        return;
    }

    if (socketSelector.wait(sf::seconds(timeout)))
    {
        if (socketSelector.isReady(socket))
        {
            setReady(true);
            if (socket.receive(pktBuf, ipBuffer, portBuf) == sf::Socket::Status::Done)
            {
                std::string header;
                pktBuf >> header;

                if (header == "ACK")
                {
                    handleServerAck(ipBuffer.value(), portBuf);
                }
                else if (header == "RCN")
                {
                    handleServerRcn(ipBuffer.value(), portBuf);
                }
                else if (header == "RFS")
                {
                    handleServerRfs(ipBuffer.value(), portBuf);
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
        logger.logError(_("Connection timeout: ") + ip.toString() + ":" + std::to_string(port), false);
        setReady(true);
        setStatus(ClientStatus::TimedOut);
    }

    socket.setBlocking(false);
}

void Client::listenerThread()
{
    sf::Packet pktBuf;

    while (status == ClientStatus::Connected)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (socketSelector.wait(sf::seconds(5.f)))
        {
            if (socketSelector.isReady(socket))
            {
                std::optional<sf::IpAddress> ip;

                if (socket.receive(pktBuf, ip, portBuf) == sf::Socket::Status::Done)
                {
                    if (ip.value() != serverIp || portBuf != serverPort)
                        continue;

                    std::pair<PacketAddress, sf::Packet> packet({ip.value(), portBuf}, sf::Packet(pktBuf));
                    packetQueue.push(packet);

                    pktBuf.clear();

                    handler();
                }
            }
        }
        else
        {
            logger.logInfo(_("Connection with server ") + serverIp.toString() + _(" timed out after 5 seconds."));
            disconnect();
        }
    }
}

void Client::handler()
{
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
            receiveFile("Assets/Client/", pkt);
        }
    }
}

void Client::handleServerAck(const sf::IpAddress &ip, const unsigned short &port)
{
    logger.logInfo(_("Connected to server: ") + ip.toString() + ":" + std::to_string(port) + ".");
    serverIp = ip;
    serverPort = port;
    setStatus(ClientStatus::Connected);
    std::thread(&Client::listenerThread, this).detach();
}

void Client::handleServerRcn(const sf::IpAddress &ip, const unsigned short &port)
{
    logger.logInfo(_("Reconnected to server: ") + ip.toString() + ":" + std::to_string(port) + ".");
    serverIp = ip;
    serverPort = port;
    setStatus(ClientStatus::Connected);

    std::thread(&Client::listenerThread, this).detach();
}

void Client::handleServerRfs(const sf::IpAddress &ip, const unsigned short &port)
{
    logger.logError(_("Connection refused by server ") + ip.toString() + ":" + std::to_string(port) + ".", false);
    setStatus(ClientStatus::Refused);
}

void Client::handleServerBadResponse(const sf::IpAddress &ip, const unsigned short &port)
{
    logger.logError(_("Bad response from server ") + ip.toString() + ":" + std::to_string(port) + ".", false);
    setStatus(ClientStatus::Bad);
}

void Client::setStatus(const ClientStatus &status)
{
    this->status = status;
}

void Client::setReady(const bool &ready)
{
    this->ready = ready;
}

Client::Client(const std::string &uuid)
    : myUuid(uuid), logger("Client"), serverIp(0, 0, 0, 0), serverPort(0), ready(true), status(ClientStatus::None)
{
    socket.setBlocking(false);
    socketSelector.add(socket);

    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
    {
        logger.logError(_("Could not bind to a port."), false);
        setStatus(ClientStatus::SockError);
        return;
    }
}

Client::~Client() = default;

void Client::connect(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    if (status == ClientStatus::Connected)
    {
        logger.logWarning(_("Already connected to ") + serverIp.toString() + ":" + std::to_string(serverPort));
        return;
    }

    std::thread(&Client::connectorThread, this, ip, port, timeout).detach();
}

void Client::disconnect()
{
    if (status != ClientStatus::Connected)
    {
        logger.logError(_("Not connected to any server."), false);
        return;
    }

    sf::Packet pktBuf;

    pktBuf << "KIL" << myUuid;

    if (!send(pktBuf))
        logger.logError(_("Failed to communicate with server. Disconnecting anyway."));

    setStatus(ClientStatus::Disconnected);
    logger.logInfo(_("Disconnected from server ") + serverIp.toString() + ":" + std::to_string(serverPort) + ".");
}

const bool Client::isReady() const
{
    return ready;
}

const ClientStatus &Client::getStatus() const
{
    return status;
}

const bool Client::send(sf::Packet &packet)
{
    if (socket.send(packet, serverIp, serverPort) != sf::Socket::Status::Done)
    {
        logger.logError(_("Error sending packet to ") + serverIp.toString() + ":" + std::to_string(serverPort), false);
        return false;
    }
    packet.clear();
    return true;
}

const bool Client::send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port)
{
    if (socket.send(packet, ip, port) != sf::Socket::Status::Done)
    {
        logger.logError(_("Error sending packet to ") + ip.toString() + ":" + std::to_string(port), false);

        return false;
    }
    return true;
}

void Client::sendFile(const std::filesystem::path &path, std::ios::openmode &mode)
{
    using namespace std::chrono_literals;

    if (status != ClientStatus::Connected)
        logger.logError(_("Not connected to any server."));

    if (!File::validatePath(path))
        logger.logError(_("File \"") + path.string() + _("\" does not exist."));

    File::FileDescriptor fd = File::createFileDescriptor(path, mode);
    const size_t HEADER_SIZE = sizeof("FILE") + sizeof(fd);

    std::ifstream file(path, mode);

    for (int i = 0; i < fd.total_parts; ++i)
    {
        sf::Packet packet;
        packet << "FILE" << fd;

        unsigned int bytes = 0;
        if (mode == std::ios::binary)
        {
            std::uint8_t byte;
            while (bytes < sf::UdpSocket::MaxDatagramSize - HEADER_SIZE)
            {
                if (!file.read(reinterpret_cast<char *>(&byte), sizeof(byte)))
                    break;

                packet << byte;
                ++bytes;
            }
        }
        else
        {
            char c;
            while (bytes < sf::UdpSocket::MaxDatagramSize - HEADER_SIZE)
            {
                if (!file.get(c))
                    break;

                packet << c;
                ++bytes;
            }
        }

        if (!send(packet, serverIp, serverPort))
        {
            logger.logError(_("Error sending file part ") + std::to_string(fd.part) + "/" +
                            std::to_string(fd.total_parts) + " (" + std::to_string(bytes) + _(" B) of file ") +
                            fd.filename + _("to: ") + serverIp.toString() + ":" + std::to_string(serverPort));
        }
        else
        {
            logger.logInfo(_("Sent file part: ") + std::to_string(fd.part) + "/" + std::to_string(fd.total_parts) +
                           " (" + std::to_string(bytes) + _(" B) of file ") + fd.filename);
            ++fd.part;
            std::this_thread::sleep_for(20ms); // Reduce network congestion
        }
    }

    file.close();
}

void Client::receiveFile(const std::filesystem::path &folder, sf::Packet &packet)
{
    if (!File::validatePath(folder))
        std::filesystem::create_directory(folder);

    File::FileDescriptor fd;
    packet >> fd;

    std::ios::openmode modes = fd.part > 1 ? static_cast<std::ios::openmode>(fd.mode) | std::ios::app
                                           : static_cast<std::ios::openmode>(fd.mode);

    std::ofstream file(folder / fd.filename, modes);
    if (!file.is_open())
    {
        logger.logError(_("Could not write to file: \"") + (folder / fd.filename).string() + "\"");
    }

    if (static_cast<std::ios::openmode>(fd.mode) == std::ios::binary)
    {
        std::uint8_t byte;
        while (packet >> byte)
        {
            file.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
        }
    }
    else
    {
        std::uint8_t c;
        while (packet >> c)
        {
            file.put(static_cast<char>(c));
        }
    }

    file.close();
    logger.logInfo(_("Received file part ") + std::to_string(fd.part) + "/" + std::to_string(fd.total_parts) +
                   _(" of file: ") + fd.filename);
}

std::optional<std::pair<PacketAddress, sf::Packet>> Client::consumePacket()
{
    if (packetQueue.empty())
        return std::nullopt;

    auto packet = std::move(packetQueue.front());
    packetQueue.pop();
    return packet;
}
