#include "Network/Server.hxx"
#include "stdafx.hxx"

/* THREADS ================================================================================================== */

void Server::listenerThread()
{
    while (online)
    {
        std::lock_guard<std::mutex> lock(mutex);

        handleTimedOutConnections();

        if (socketSelector.wait(sf::seconds(10.f)) && socketSelector.isReady(socket))
        {
            pktBuf.clear();
            std::optional<sf::IpAddress> ip;

            if (socket.receive(pktBuf, ip, portBuf) == sf::Socket::Status::Done && ip)
            {
                ipBuf = ip.value();

                std::pair<PacketAddress, sf::Packet> packet({ip.value(), portBuf}, sf::Packet(pktBuf));
                packetQueue.push(packet);
            }
        }
    }
}

void Server::handlerThread()
{
    while (online)
    {
        std::lock_guard<std::mutex> lock(mutex);

        for (auto opt = consumePacket(); opt.has_value(); opt = consumePacket())
        {
            auto &[pkt_addr, pkt] = opt.value();

            std::string header;
            ConnectionUID uid;
            pkt >> header >> uid;

            std::cout << "server: " << header << "\n";

            if (header == "ASK")
            {
                handleAsk(pkt_addr.ip, pkt_addr.port);
            }
            else if (header == "UID+ACK")
            {
                handleUidAck(uid);
            }
            else if (!isClientConnected(pkt_addr.ip, pkt_addr.port))
            {
                continue;
            }
            else if (header == "KIL")
            {
                disconnectClient(pkt_addr.ip);
            }
            else if (header == "FILE")
            {
                receiveFile(pkt_addr.ip, pkt_addr.port, "Assets/Server");
            }
        }
    }
}

/* HANDLERS ================================================================================================= */

void Server::handleTimedOutConnections()
{
    std::vector<sf::IpAddress> timed_out_connections;

    for (auto &[uid, conn] : connections)
    {
        if (conn.timeoutClock.getElapsedTime().asSeconds() >= conn.timeout)
        {
            logger.logInfo("Connection with client " + conn.ip.toString() + " timed out after " +
                           std::to_string(conn.timeoutClock.getElapsedTime().asSeconds()) + " seconds.");
            timed_out_connections.push_back(conn.ip);
        }
    }

    for (auto &ip : timed_out_connections)
        disconnectClient(ip);
}

void Server::handleAsk(const sf::IpAddress &ip, const unsigned short &port)
{
    const ConnectionUID uid = generateConnectionUID();

    if (!createConnection(ip, port, uid))
    {
        sendControlMessage("RFS", ip, port);
    }
    else
    {
        pktBuf.clear();
        pktBuf << "ACK+UID" << uid;
        if (!send(pktBuf, ip, port))
            logger.logError("Could not send UID to new client: " + ip.toString());
    }
}

void Server::handleUidAck(const ConnectionUID &uid)
{
}

const ConnectionUID Server::generateConnectionUID()
{
    ConnectionUID uid;
    do
    {
        uid = std::rand() % 0xFFFFFFFF;
    } while (uid == 0 || connections.count(uid));
    return uid;
}

/* CONSTRUCTOR ============================================================================================== */

Server::Server() : logger("Server"), online(false), ipBuf(0, 0, 0, 0)
{
}

Server::~Server()
{
}

/* PUBLIC METHODS =========================================================================================== */

void Server::listen(const unsigned short port)
{
    socket.setBlocking(false);

    if (socket.bind(port) != sf::Socket::Status::Done)
        logger.logError("Could not bind to port " + std::to_string(port));

    socketSelector.add(socket);

    online = true;

    std::thread(&Server::listenerThread, this).detach();
    std::thread(&Server::handlerThread, this).detach();

    logger.logInfo("[ Server::listen ] -> Server (" + sf::IpAddress::getLocalAddress()->toString() + ":" +
                   std::to_string(socket.getLocalPort()) + ") online");
}

const bool Server::createConnection(const sf::IpAddress &ip, const unsigned short &port, const ConnectionUID &uid)
{
    for (const auto &[_, conn] : connections)
    {
        if (conn.ip == ip)
        {
            logger.logError("Client with IP is already connected: " + ip.toString());
            return false;
        }
    }

    connections[uid] = {ip, port, 10.f, sf::Clock(), false};
    logger.logInfo("Client with IP " + ip.toString() + " is now connected.");
    return true;
}

void Server::disconnectClient(const sf::IpAddress &ip)
{
    auto it =
        std::find_if(connections.begin(), connections.end(), [&](const auto &pair) { return pair.second.ip == ip; });

    if (it != connections.end())
    {
        sendControlMessage("KIL", it->second.ip, it->second.port);
        connections.erase(it);
        logger.logInfo("Client with IP " + ip.toString() + " is now disconnected.");
    }
    else
    {
        logger.logError("Client with IP is not connected: " + ip.toString());
    }
}

const bool Server::isClientConnected(const sf::IpAddress &ip, const unsigned short &port)
{
    return std::any_of(connections.begin(), connections.end(), [&](const auto &pair) { return pair.second.ip == ip; });
}

const bool Server::send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port)
{
    if (socket.send(packet, ip, port) != sf::Socket::Status::Done)
    {
        logger.logError("Could not send packet to: " + ip.toString() + std::to_string(port));
        pktBuf.clear();
        return false;
    }
    pktBuf.clear();
    return true;
}

void Server::sendControlMessage(const std::string header, const sf::IpAddress &ip, const unsigned short &port)
{
    pktBuf.clear();
    pktBuf << header;
    if (!send(pktBuf, ip, port))
        logger.logError("Error while sending control message to: " + ip.toString());
}

void Server::sendFile(const sf::IpAddress &ip, const unsigned short &port, std::filesystem::path path,
                      std::ios::openmode mode)
{
    if (!isClientConnected(ip, port))
    {
        logger.logError("Client is not connected: " + ip.toString());
        return;
    }

    if (!File::validatePath(path))
    {
        logger.logError("Invalid file: " + path.string());
        return;
    }

    File::FileDescriptor f_desc = File::createFileDescriptor(path, mode);

    if (f_desc.filesize > sf::UdpSocket::MaxDatagramSize)
        throw std::runtime_error("File too big: " + path.string());

    pktBuf.clear();
    pktBuf << "FILE" << f_desc;

    std::ifstream file(path, mode);
    if (mode == std::ios::binary)
    {
        std::uint8_t byte;
        while (file.read(reinterpret_cast<char *>(&byte), sizeof(byte)))
            pktBuf << byte;
    }
    else
    {
        char c;
        while (file.get(c))
            pktBuf << c;
    }

    file.close();

    if (!send(pktBuf, ip, port))
        logger.logError("Error sending file to: " + ip.toString() + std::to_string(port));
    else
        logger.logInfo("Sent file " + f_desc.filename);
}

void Server::receiveFile(const sf::IpAddress &ip, const unsigned short &port, std::filesystem::path folder)
{
    if (!isClientConnected(ip, port))
        throw std::runtime_error("Client not connected: " + ip.toString());

    if (!std::filesystem::exists(folder))
        std::filesystem::create_directory(folder);

    std::string path_str = folder.string();
    if (path_str.back() != '/')
        path_str.push_back('/');

    File::FileDescriptor f_desc;
    pktBuf >> f_desc;

    std::ofstream file(path_str + f_desc.filename, static_cast<std::ios::openmode>(f_desc.mode));
    if (!file.is_open())
        throw std::runtime_error("Could not write file: " + path_str + f_desc.filename);

    if (static_cast<std::ios::openmode>(f_desc.mode) == std::ios::binary)
    {
        std::uint8_t byte;
        while (pktBuf >> byte)
            file.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
    }
    else
    {
        std::uint8_t c;
        while (pktBuf >> c)
            file.put(static_cast<char>(c));
    }

    pktBuf.clear();
    file.close();
    logger.logInfo("Received file: " + f_desc.filename);
}

void Server::shutdown()
{
    if (!online)
    {
        logger.logError("Server is not online");
        return;
    }

    for (const auto &[uid, conn] : connections)
        sendControlMessage("KIL", conn.ip, conn.port);

    online = false;
    socket.unbind();
    socketSelector.clear();
    logger.logInfo("Server is down");
}

std::optional<std::pair<PacketAddress, sf::Packet>> Server::consumePacket()
{
    if (packetQueue.empty())
        return std::nullopt;

    auto packet = packetQueue.front();
    packetQueue.pop();
    return packet;
}