#include "Network/Server.hxx"
#include "stdafx.hxx"

/* THREADS ================================================================================================== */

void Server::listenerThread()
{
    logger.logInfo("[ Server::listenerThread ] -> Server (" + sf::IpAddress::getLocalAddress()->toString() + ":" +
                   std::to_string(socket.getLocalPort()) + ") online.");

    while (online)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            handleTimedOutConnections();
        }

        if (socketSelector.wait(sf::seconds(10.f)) && socketSelector.isReady(socket))
        {
            pktBuf.clear();
            std::optional<sf::IpAddress> ip;
            unsigned short port;

            if (socket.receive(pktBuf, ip, port) == sf::Socket::Status::Done)
            {
                std::lock_guard<std::mutex> lock(mutex);
                packetQueue.emplace(PacketAddress{*ip, port}, std::move(pktBuf));
                handler();
            }
        }
    }

    logger.logInfo("[ Server::listenerThread ] -> Server (" + sf::IpAddress::getLocalAddress()->toString() + ":" +
                   std::to_string(socket.getLocalPort()) + ") offline.");
}

void Server::handler()
{
    while (auto opt = consumePacket())
    {
        auto &[pkt_addr, pkt] = *opt;

        std::string header, uuid;
        pkt >> header >> uuid;

        if (header == "ASK+UUID")
        {
            handleAskUuid(uuid, pkt_addr.ip, pkt_addr.port);
        }
        else if (!isClientConnected(pkt_addr.ip, pkt_addr.port))
        {
            continue;
        }
        else if (header == "KIL")
        {
            disconnectClient(uuid);
        }
        else if (header == "FILE")
        {
            receiveFile(pkt_addr.ip, pkt_addr.port, "Assets/Server");
        }
    }
}

/* HANDLERS ================================================================================================= */

void Server::handleTimedOutConnections()
{
    std::vector<std::string> timed_out_connections;
    timed_out_connections.reserve(connections.size());

    for (const auto &[uuid, conn] : connections)
    {
        if (conn.active && conn.timeoutClock.getElapsedTime().asSeconds() >= conn.timeout)
        {
            logger.logInfo("Connection with client " + conn.ip.toString() + " timed out after " +
                           std::to_string(conn.timeout) + " seconds.");
            timed_out_connections.push_back(uuid);
        }
    }

    for (const auto &uuid : timed_out_connections)
    {
        disconnectClient(uuid);
    }
}

void Server::handleAskUuid(const std::string &uuid, const sf::IpAddress &ip, const unsigned short port)
{
    if (uuid == myUuid)
    {
        logger.logError("Connecting to self is not allowed.", false);
        sendControlMessage("RFS", ip, port);
        return;
    }

    auto it = connections.find(uuid);
    if (it != connections.end())
    {
        if (!it->second.active)
        {
            logger.logInfo("Client with IP reconnected: " + ip.toString());
            it->second = {ip, port, 10.f, sf::Clock(), true};
            sendControlMessage("ACK", ip, port);
        }
        else
        {
            logger.logError("Client with IP is already connected: " + ip.toString(), false);
        }
    }
    else if (!createConnection(ip, port, uuid))
    {
        sendControlMessage("RFS", ip, port);
    }
    else
    {
        sendControlMessage("ACK", ip, port);
    }
}

void Server::setOnline(bool online)
{
    this->online = online;
}

/* CONSTRUCTOR ============================================================================================== */

Server::Server(const std::string &uuid) : myUuid(uuid), logger("Server"), online(false)
{
    socket.setBlocking(true);
}

Server::~Server() = default;

/* PUBLIC METHODS =========================================================================================== */

void Server::listen(const unsigned short port)
{
    if (socket.bind(port) != sf::Socket::Status::Done)
    {
        logger.logError("Could not bind to port " + std::to_string(port));
        return;
    }

    socketSelector.add(socket);
    setOnline(true);

    std::thread(&Server::listenerThread, this).detach();
}

bool Server::createConnection(const sf::IpAddress &ip, const unsigned short port, const std::string &uuid)
{
    if (connections.find(uuid) != connections.end())
    {
        logger.logError("Client with IP is already connected: " + ip.toString(), false);
        return false;
    }

    connections[uuid] = {ip, port, 10.f, sf::Clock(), true};
    logger.logInfo("Client with IP " + ip.toString() + " is now connected.");
    return true;
}

void Server::disconnectClient(const std::string &uuid)
{
    auto it = connections.find(uuid);
    if (it == connections.end())
    {
        logger.logError("Client with UUID " + uuid + " is not connected.", false);
        return;
    }

    sendControlMessage("KIL", it->second.ip, it->second.port);
    connections.erase(it);
    logger.logInfo("Client with IP " + it->second.ip.toString() + " is now disconnected.");
}

bool Server::isClientConnected(const sf::IpAddress &ip, const unsigned short port) const
{
    return std::find_if(connections.begin(), connections.end(), [&](const auto &pair) {
               return pair.second.ip == ip && pair.second.port == port;
           }) != connections.end();
}

bool Server::send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short port)
{
    if (socket.send(packet, ip, port) != sf::Socket::Status::Done)
    {
        logger.logError("Could not send packet to: " + ip.toString() + ":" + std::to_string(port));
        return false;
    }
    return true;
}

void Server::sendControlMessage(const std::string &header, const sf::IpAddress &ip, const unsigned short port)
{
    sf::Packet packet;
    packet << header;
    if (!send(packet, ip, port))
    {
        logger.logError("Error while sending control message to: " + ip.toString());
    }
}

void Server::sendFile(const sf::IpAddress &ip, const unsigned short port, const std::filesystem::path &path,
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
    {
        throw std::runtime_error("File too big: " + path.string());
    }

    sf::Packet packet;
    packet << "FILE" << f_desc;

    std::ifstream file(path, mode);
    if (mode == std::ios::binary)
    {
        std::uint8_t byte;
        while (file.read(reinterpret_cast<char *>(&byte), sizeof(byte)))
        {
            packet << byte;
        }
    }
    else
    {
        char c;
        while (file.get(c))
        {
            packet << c;
        }
    }

    file.close();

    if (!send(packet, ip, port))
    {
        logger.logError("Error sending file to: " + ip.toString() + ":" + std::to_string(port));
    }
    else
    {
        logger.logInfo("Sent file " + f_desc.filename);
    }
}

void Server::receiveFile(const sf::IpAddress &ip, const unsigned short port, const std::filesystem::path &folder)
{
    if (!isClientConnected(ip, port))
    {
        throw std::runtime_error("Client not connected: " + ip.toString());
    }

    if (!std::filesystem::exists(folder))
    {
        std::filesystem::create_directory(folder);
    }

    std::string path_str = folder.string();
    if (path_str.back() != '/')
    {
        path_str.push_back('/');
    }

    File::FileDescriptor f_desc;
    pktBuf >> f_desc;

    std::ofstream file(path_str + f_desc.filename, static_cast<std::ios::openmode>(f_desc.mode));
    if (!file.is_open())
    {
        throw std::runtime_error("Could not write file: " + path_str + f_desc.filename);
    }

    if (static_cast<std::ios::openmode>(f_desc.mode) == std::ios::binary)
    {
        std::uint8_t byte;
        while (pktBuf >> byte)
        {
            file.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
        }
    }
    else
    {
        std::uint8_t c;
        while (pktBuf >> c)
        {
            file.put(static_cast<char>(c));
        }
    }

    pktBuf.clear();
    file.close();
    logger.logInfo("Received file: " + f_desc.filename);
}

void Server::shutdown()
{
    if (!online)
    {
        logger.logError("Server is not online", false);
        return;
    }

    for (const auto &[uuid, conn] : connections)
    {
        sendControlMessage("KIL", conn.ip, conn.port);
    }

    online = false;
    socket.unbind();
    socketSelector.clear();
    logger.logInfo("Server is down");
}

std::optional<std::pair<PacketAddress, sf::Packet>> Server::consumePacket()
{
    if (packetQueue.empty())
    {
        return std::nullopt;
    }

    auto packet = std::move(packetQueue.front());
    packetQueue.pop();
    return packet;
}