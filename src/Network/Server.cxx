#include "Network/Server.hxx"
#include "stdafx.hxx"

void Server::listenerThread()
{
    while (online)
    {
        mutex.lock();
        if (socketSelector.wait(sf::seconds(10.f)))
        {
            if (socketSelector.isReady(socket))
            {
                packetBuffer.clear();
                std::optional<sf::IpAddress> ip;

                if (socket.receive(packetBuffer, ip, portBuffer) == sf::Socket::Status::Done)
                {
                    ipBuffer = ip.value();

                    std::string header;
                    ConnectionUID uid;
                    packetBuffer >> header >> uid;

                    if (header == "ASK") // Handle player connection
                    {
                        uid = generateConnectionUID();

                        if (!createConnection(ipBuffer, portBuffer, uid))
                            sendControlMessage("RFS", ipBuffer, portBuffer);
                        else
                        {
                            packetBuffer.clear();
                            packetBuffer << "ACK+UID" << uid;

                            if (!send(packetBuffer, ipBuffer, portBuffer))
                                std::cerr << "[ Server ] -> Could not send UID to new client: " + ipBuffer.toString()
                                          << ".\n";
                        }
                    }
                    else if (header == "UID+ACK")
                    {
                        try
                        {
                            connections.at(uid).active = true;
                            std::cout << "[ Server ] -> Client with UID " << uid << " has now an active connection."
                                      << "\n";
                        }
                        catch (std::exception &)
                        {
                            std::cerr << "[ Server ] -> Unknown host trying to confirm connection: "
                                      << ipBuffer.toString() << ":" << portBuffer << ".\n";
                        }
                    }
                    else if (!isClientConnected(ipBuffer, portBuffer))
                    {
                        mutex.unlock();
                        continue;
                    }
                    else if (header == "KIL") // Handle player disconnection
                    {
                        disconnectClient(ipBuffer);
                    }
                    else if (header == "FILE")
                    {
                        receiveFile(ipBuffer, portBuffer, "Assets/Server");
                    }
                    else
                    {
                        packetQueue.push(packetBuffer); // Send to queue
                    }
                }
            }
        }
        mutex.unlock();
    }
}

const ConnectionUID Server::generateConnectionUID()
{
    ConnectionUID uid = std::rand() % 0xFFFFFFFF;

    while (uid == 0 || connections.count(uid) != 0)
        uid = std::rand() % 0xFFFFFFFF;

    return uid;
}

Server::Server() : online(false), ipBuffer(0, 0, 0, 0)
{
}

Server::~Server()
{
}

void Server::listen(const unsigned short port)
{
    socket.setBlocking(false);

    if (socket.bind(port) != sf::Socket::Status::Done)
        throw std::runtime_error("[ Server::listen ] -> Could not bind to port " + std::to_string(port) + "\n");

    socketSelector.add(socket);

    std::thread(&Server::listenerThread, this).detach();

    online = true;

    std::cout << "[ Server::listen ] -> Server (" << sf::IpAddress::getLocalAddress()->toString() << ":"
              << std::to_string(socket.getLocalPort()) << ") online" << "\n";
}

const bool Server::createConnection(const sf::IpAddress &ip, const unsigned short &port, const ConnectionUID &uid)
{
    Connection connection{ip, port};
    connection.timeoutClock.restart();
    connection.active = false;

    for (auto &[uid, conn] : connections)
    {
        if (conn.ip.toInteger() == connection.ip.toInteger())
        {
            std::cerr << "[ Server::createConnection ] -> Client with IP " << ip.toString() << " is already connected."
                      << "\n";

            return false;
        }
    }

    connections[uid] = connection; // Link IP and Port

    std::cout << "[ Server::createConnection ] -> Client with IP " << ip.toString() << " is now connected."
              << "\n";

    return true;
}

void Server::disconnectClient(const sf::IpAddress &ip)
{
    ConnectionUID conn_uid = 0;

    for (auto &[uid, conn] : connections)
    {
        if (conn.ip == ip)
        {
            conn_uid = uid;
            break;
        }
    }

    if (conn_uid != 0)
    {
        connections.erase(conn_uid);
        std::cout << "[ Server::disconnectClient ] -> Client with IP " << ip.toString() << " is now disconnected."
                  << "\n";
    }
    else
    {
        std::cerr << "[ Server::disconnectClient ] -> Client with IP " << ip.toString() << " is not connected."
                  << "\n";
    }
}

const bool Server::isClientConnected(const sf::IpAddress &ip, const unsigned short &port)
{
    for (auto &[uid, connection] : connections)
    {
        if (connection.ip == ip)
            return true;
    }

    return false;
}

const bool Server::send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port)
{
    if (socket.send(packet, ip, port) != sf::Socket::Status::Done)
    {
        std::cerr << "[ Server::send ] -> Could not send packet to " << ipBuffer.toString() << ":"
                  << std::to_string(portBuffer) << "\n";

        packetBuffer.clear();
        return false;
    }

    packetBuffer.clear();
    return true;
}

void Server::sendControlMessage(const std::string header, const sf::IpAddress &ip, const unsigned short &port)
{
    packetBuffer.clear();
    packetBuffer << header;

    if (!send(packetBuffer, ip, port))
        std::cerr << "[ Server::sendControlMessage ] -> Error while sending control message to " << ip.toString()
                  << ".\n";
}

void Server::sendFile(const sf::IpAddress &ip, const unsigned short &port, std::filesystem::path path,
                      std::ios::openmode mode)
{
    if (!isClientConnected(ip, port))
        throw std::runtime_error("[ Server::sendFile ] -> Client " + ip.toString() = " is not connected.\n");

    if (!std::filesystem::exists(path))
        throw std::runtime_error("[ Server::sendFile ] -> File \"" + path.string() = "\" does not exist.\n");

    std::ifstream file(path, mode);

    if (!file.is_open())
        throw std::runtime_error("[ Server::sendFile ] -> Could not open file \"" + path.string() = "\"\n");

    FileDescriptor f_desc;

    f_desc.filename = path.filename().string();
    f_desc.filesize = std::filesystem::file_size(path);
    f_desc.mode = static_cast<int>(mode);
    f_desc.part = 1;
    f_desc.total_parts = 1;

    if (f_desc.filesize > sf::UdpSocket::MaxDatagramSize) // TODO: Segment files bigger than MaxDatagramSize
        throw std::runtime_error("[ Server::sendFile ] -> File \"" + path.string() =
                                     "\" too big (" + std::to_string(f_desc.filesize) + " bytes)\n");

    packetBuffer.clear();
    packetBuffer << "FILE";
    packetBuffer << f_desc;

    if (mode == std::ios::binary)
    {
        std::uint8_t byte;
        while (!file.eof())
        {
            file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
            packetBuffer << byte;
        }
    }
    else
    {
        char c;
        while (file >> std::noskipws >> c)
            packetBuffer << c;
    }

    file.close();

    if (!send(packetBuffer, ip, port))
        std::cerr << "[ Server::sendFile ] -> Error sending file \"" << f_desc.filename << "\" to " << ip.toString()
                  << ":" << port << "\n";
    else
        std::cout << "[ Server::sendFile ] -> Sent file \"" << f_desc.filename << "\" (" << f_desc.filesize
                  << " bytes) to " << ip.toString() << ":" << port << "\n";
}

void Server::receiveFile(const sf::IpAddress &ip, const unsigned short &port, std::filesystem::path folder)
{
    if (!isClientConnected(ip, port))
        throw std::runtime_error("[ Server::sendFile ] -> Client " + ip.toString() = " is not connected.\n");

    if (!std::filesystem::exists(folder))
        std::filesystem::create_directory(folder);

    std::string path_str = folder.string();
    if (path_str.back() != '/')
        path_str.push_back('/');

    FileDescriptor f_desc;
    packetBuffer >> f_desc;

    std::ofstream file(path_str + f_desc.filename, static_cast<std::ios::openmode>(f_desc.mode));

    if (!file.is_open())
        throw std::runtime_error("[ Server::receiveFile ] -> Could not write file \"" + path_str + f_desc.filename +
                                 "\".\n");

    if (static_cast<std::ios::openmode>(f_desc.mode) == std::ios::binary)
    {
        std::uint8_t byte;

        while (packetBuffer >> byte)
            file.write(reinterpret_cast<char *>(&byte), sizeof(byte));
    }
    else
    {
        std::uint8_t c;

        while (packetBuffer >> c)
            file.write(reinterpret_cast<char *>(c), sizeof(c));
    }

    packetBuffer.clear();
    file.close();

    std::cout << "[ Server::receiveFile ] -> Received file \"" << path_str + f_desc.filename << "\" ("
              << f_desc.filesize << " bytes) to " << ip.toString() << ":" << port << ".\n";
}

void Server::shutdown()
{
    if (!online)
    {
        std::cerr << "[ Server::shutdown ] -> Server is not online." << "\n";
        return;
    }

    unsigned short port = socket.getLocalPort();

    for (auto &[uid, conn] : connections)
        sendControlMessage("KIL", conn.ip, conn.port);

    online = false;
    socket.unbind();
    socketSelector.clear();

    std::cout << "[ Server::shutdown ] -> Server (" << sf::IpAddress::getLocalAddress()->toString() << ":"
              << std::to_string(port) << ") is down." << "\n";
}

std::optional<sf::Packet> Server::consumePacket()
{
    std::optional<sf::Packet> packet;

    if (!packetQueue.empty())
    {
        packet.emplace(packetQueue.front());
        packetQueue.pop();
    }

    return packet;
}