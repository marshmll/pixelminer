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
                    packetBuffer >> header;

                    if (header == "CON") // Handle player connection
                    {
                        if (!connectClient(ipBuffer, portBuffer))
                            sendControlMessage("RFS", ipBuffer, portBuffer);
                        else
                        {
                            sendControlMessage("ACK", ipBuffer, portBuffer);
                        }
                    }
                    else if (clients.count(ipBuffer) == 0)
                    {
                        std::cerr << "[ Server::listenerThread ] -> Client with IP " << ipBuffer.toString()
                                  << " is not connected."
                                  << "\n";

                        mutex.unlock();
                        continue; // Ignore clients that did not ask to connect
                    }
                    else if (header == "KIL") // Handle player disconnection
                    {
                        disconnectClient(ipBuffer);
                    }
                    else if (header == "FIL")
                    {
                        receiveFile(ipBuffer, "Assets/Server");
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

const bool Server::connectClient(const sf::IpAddress &ip, const unsigned short &port)
{
    if (clients.count(ip) != 0)
    {
        std::cerr << "[ Server::subscribeClient ] -> Client with IP " << ip.toString() << " is already connected."
                  << "\n";
        return false;
    }
    else
    {
        clients[ip] = port; // Link IP and Port
        std::cout << "[ Server::subscribeClient ] -> Client with IP " << ip.toString() << " is now connected."
                  << "\n";
        return true;
    }
}

void Server::disconnectClient(const sf::IpAddress &ip)
{
    try
    {
        clients.erase(ip);
        std::cout << "[ Server::subscribeClient ] -> Client with IP " << ip.toString() << " is now disconnected."
                  << "\n";
    }
    catch (std::exception e)
    {
        std::cerr << "[ Server::unsubscribeClient ] -> Client with IP " << ip.toString() << " is not connected."
                  << "\n";
    }
}

void Server::sendControlMessage(const std::string header, const sf::IpAddress &ip, const unsigned short &port)
{
    try
    {
        packetBuffer.clear();
        packetBuffer << header;

        if (socket.send(packetBuffer, ip, port) != sf::Socket::Status::Done)
        {
            std::cerr << "[ Server::sendControlMessage ] -> Could not send control message to " << ipBuffer.toString()
                      << ":" << std::to_string(portBuffer) << "\n";
        }

        packetBuffer.clear();
    }
    catch (std::exception e)
    {
        std::cerr << "[ Server::sendControlMessage ] -> Error while sending control message to " << ip.toString()
                  << ": " << e.what() << "\n";
    }
}

void Server::sendFile(const sf::IpAddress client, std::filesystem::path path, std::ios::openmode mode)
{
    if (clients.count(client) == 0)
        throw std::runtime_error("[ Server::sendFile ] -> Client " + client.toString() = " is not connected.\n");

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
    packetBuffer << "FIL";
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

    if (socket.send(packetBuffer, client, clients.at(client)) != sf::Socket::Status::Done)
        throw std::runtime_error("[ Server::sendFile ] -> Error sending file \"" + path.string() = "\"\n");

    packetBuffer.clear();

    std::cout << "[ Server::sendFile ] -> Sent file \"" << f_desc.filename << "\" (" << f_desc.filesize << " bytes) to "
              << client.toString() << ":" << clients.at(client) << "\n";
}

void Server::receiveFile(const sf::IpAddress client, std::filesystem::path folder)
{
    if (clients.count(client) == 0)
        throw std::runtime_error("[ Server::sendFile ] -> Client " + client.toString() = " is not connected.\n");

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
                                 "\"\n");

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
              << f_desc.filesize << " bytes) to " << client.toString() << ":" << clients.at(client) << "\n";
}

void Server::shutdown()
{
    if (!online)
    {
        std::cerr << "[ Server::shutdown ] -> Server is not online" << "\n";
        return;
    }

    unsigned short port = socket.getLocalPort();

    for (auto &[ip, port] : clients)
    {
        std::cout << "[ Server::shutdown ] -> Killing connection with " << ip.toString() << ":" << std::to_string(port)
                  << "\n";
        sendControlMessage("KIL", ip, port);
    }

    online = false;
    socket.unbind();
    socketSelector.clear();

    std::cout << "[ Server::shutdown ] -> Server (" << sf::IpAddress::getLocalAddress()->toString() << ":"
              << std::to_string(port) << ") offline" << "\n";
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