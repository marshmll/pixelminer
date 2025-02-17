#include "Network/Server.hxx"
#include "stdafx.hxx"

/* THREADS ================================================================================================== */

void Server::listenerThread()
{
    logger.logInfo("Server (" + sf::IpAddress::getLocalAddress()->toString() + ":" +
                   std::to_string(socket.getLocalPort()) + ") online.");

    sf::Packet pktBuf;

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

    std::cout << std::flush;

    logger.logInfo(_("Server's listener thread for (") + sf::IpAddress::getLocalAddress()->toString() +
                   _(") is offline."));
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
        else if (header == "INFO")
        {
            sendServerInfo(pkt_addr.ip, pkt_addr.port);
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
            receiveFile(pkt_addr.ip, pkt_addr.port, "Assets/Server", pkt);
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
            logger.logInfo(_("Connection with client ") + conn.ip.toString() + _(" timed out after ") +
                           std::to_string(conn.timeout) + _(" seconds."));
            timed_out_connections.push_back(uuid);
        }
    }

    for (const auto &uuid : timed_out_connections)
    {
        disconnectClient(uuid);
    }
}

void Server::handleAskUuid(const std::string &uuid, const sf::IpAddress &ip, const unsigned short &port)
{
    if (uuid == myUuid)
    {
        logger.logError(_("Connecting to self is not allowed."), false);
        sendControlMessage("RFS", ip, port);
        return;
    }

    auto it = connections.find(uuid);
    if (it != connections.end())
    {
        if (!it->second.active)
        {
            logger.logInfo(_("Client with IP reconnected: ") + ip.toString());
            it->second = {ip, port, 10.f, sf::Clock(), true};
            sendControlMessage("ACK", ip, port);
        }
        else
        {
            logger.logError(_("Client with IP is already connected: ") + ip.toString(), false);
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

void Server::sendServerInfo(const sf::IpAddress &ip, const unsigned short &port)
{
    JObject server_info_obj(
        {{"name", "Pixelminer Server"},
         {"description", _("A Pixelminer Server.")},
         {"gameVersion", GAME_VERSION},
         {"address", sf::IpAddress::getLocalAddress()->toString() + ":" + std::to_string(this->socket.getLocalPort())},
         {"connections", static_cast<long long>(connections.size() + 1)},
         {"maxConnections", static_cast<long long>(maxConnections)}});

    sf::Packet packet;
    packet << "ACK+INFO" << JSON::stringify(server_info_obj);

    send(packet, ip, port);
}

void Server::setOnline(bool online)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->online = online;
}

/* CONSTRUCTOR ============================================================================================== */

Server::Server(const std::string &uuid) : myUuid(uuid), logger("Server"), maxConnections(8), online(false)
{
    socket.setBlocking(true);
    socketSelector.add(socket);
}

Server::~Server()
{
    shutdown();
}

/* PUBLIC METHODS =========================================================================================== */

const bool Server::listen(const unsigned short &port)
{
    if (online)
    {
        logger.logWarning(_("Already listening on port ") + std::to_string(socket.getLocalPort()));
        return false;
    }

    if (socket.bind(port) != sf::Socket::Status::Done)
    {
        logger.logError(_("Could not bind to port ") + std::to_string(port));
        return false;
    }

    setOnline(true);

    std::thread(&Server::listenerThread, this).detach();

    return true;
}

bool Server::createConnection(const sf::IpAddress &ip, const unsigned short &port, const std::string &uuid)
{
    if (connections.size() + 1 >= maxConnections)
    {
        logger.logWarning(_("Maximum number of connections reached. Refused connection with client: ") + ip.toString());

        return false;
    }

    if (connections.find(uuid) != connections.end())
    {
        logger.logWarning(_("Client with IP is already connected: ") + ip.toString());
        return false;
    }

    connections[uuid] = {ip, port, 10.f, sf::Clock(), true};
    logger.logInfo(_("Client with IP ") + ip.toString() + _(" connected."));
    return true;
}

void Server::disconnectClient(const std::string &uuid)
{
    auto it = connections.find(uuid);
    if (it == connections.end())
    {
        logger.logError(_("Client with UUID ") + uuid + _(" is not connected."), false);
        return;
    }

    sendControlMessage("KIL", it->second.ip, it->second.port);
    connections.erase(it);
    logger.logInfo(_("Client with IP ") + it->second.ip.toString() + _(" is now disconnected."));
}

bool Server::isClientConnected(const sf::IpAddress &ip, const unsigned short &port) const
{
    return std::find_if(connections.begin(), connections.end(), [&](const auto &pair) {
               return pair.second.ip == ip && pair.second.port == port;
           }) != connections.end();
}

const std::string Server::getFullAddress()
{
    return sf::IpAddress::getLocalAddress()->toString() + ":" + std::to_string(socket.getLocalPort());
}

bool Server::send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port)
{
    if (socket.send(packet, ip, port) != sf::Socket::Status::Done)
    {
        logger.logError(_("Could not send packet to: ") + ip.toString() + ":" + std::to_string(port));
        return false;
    }
    return true;
}

void Server::sendControlMessage(const std::string &header, const sf::IpAddress &ip, const unsigned short &port)
{
    sf::Packet packet;
    packet << header;
    if (!send(packet, ip, port))
    {
        logger.logError(_("Error while sending control message to: ") + ip.toString());
    }
}

void Server::sendFile(const sf::IpAddress &ip, const unsigned short &port, const std::filesystem::path &path,
                      std::ios::openmode mode)
{
    using namespace std::chrono_literals;

    if (!isClientConnected(ip, port))
    {
        logger.logError(_("Client is not connected: ") + ip.toString());
        return;
    }

    if (!File::validatePath(path))
    {
        logger.logError(_("Invalid file: ") + path.string());
        return;
    }

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

        if (!send(packet, ip, port))
        {
            logger.logError(_("Error sending file part ") + std::to_string(fd.part) + "/" +
                            std::to_string(fd.total_parts) + " (" + std::to_string(bytes) + _(" B) of file ") +
                            fd.filename + _("to: ") + ip.toString() + ":" + std::to_string(port));
        }
        else
        {
            logger.logInfo("Sent file part: " + std::to_string(fd.part) + "/" + std::to_string(fd.total_parts) + " (" +
                           std::to_string(bytes) + _(" B) of file ") + fd.filename);
            ++fd.part;
            std::this_thread::sleep_for(20ms); // Reduce network congestion
        }
    }

    file.close();
}

void Server::receiveFile(const sf::IpAddress &ip, const unsigned short &port, const std::filesystem::path &folder,
                         sf::Packet &packet)
{
    if (!isClientConnected(ip, port))
    {
        throw std::runtime_error(_("Client is not connected: ") + ip.toString());
    }

    if (!std::filesystem::exists(folder))
    {
        std::filesystem::create_directory(folder);
    }

    File::FileDescriptor fd;
    packet >> fd;

    std::ofstream file(folder / fd.filename, static_cast<std::ios::openmode>(fd.mode) | std::ios::app);
    if (!file.is_open())
    {
        logger.logError(_("Could not write file: ") + (folder / fd.filename).string());
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
                   _("of file: ") + fd.filename);
}

void Server::shutdown()
{
    if (!online)
        return;

    for (const auto &[uuid, conn] : connections)
    {
        sendControlMessage("KIL", conn.ip, conn.port);
    }

    setOnline(false);
    socket.unbind();
    socketSelector.clear();
    logger.logInfo(_("Server is down"));
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