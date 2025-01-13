#include "Network/Client.hxx"
#include "stdafx.hxx"

void Client::connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    mutex.lock();
    socket.setBlocking(true); // Set blocking while establishing connection.
    ready = false;

    packetBuffer.clear();
    packetBuffer << "ASK";

    if (!send(packetBuffer, ip, port))
    {
        std::cerr << "[ Client::connectorThread ] -> Could not ask for a connection with " << ip.toString() << ":"
                  << std::to_string(port) << "\n";

        ready = true;
        connected = false;
    }
    else if (socketSelector.wait(sf::seconds(timeout)))
    {
        if (socketSelector.isReady(socket))
        {
            if (socket.receive(packetBuffer, ipBuffer, portBuffer) == sf::Socket::Status::Done)
            {
                ready = true;

                std::string header;
                packetBuffer >> header;

                if (header == "ACK+UID")
                {
                    packetBuffer >> myUid;
                    handleServerACKUID(ipBuffer.value(), portBuffer);
                }
                else if (header == "RFS")
                {
                    handleServerRFS(ipBuffer.value(), portBuffer);
                }
                else
                {
                    handleServerBadResponse(ipBuffer.value(), portBuffer);
                }
            }
        }
    }
    else
    {
        std::cerr << ("[ Client::connectorThread ] -> Connection timeout: " + ip.toString() + ":" +
                      std::to_string(port) + "\n");
        ready = true;
        connected = false;
    }

    socket.setBlocking(false);
    mutex.unlock();
}

void Client::handleServerACKUID(const sf::IpAddress &ip, const unsigned short &port)
{
    std::cout << "[ Client::connectorThread ] -> Connected to server: " << ip.toString() << ":" << std::to_string(port)
              << "\n";

    serverIp = ipBuffer.value();
    serverPort = portBuffer;

    packetBuffer.clear();
    packetBuffer << "UID+ACK" << myUid;

    send(packetBuffer);

    ready = true;
    connected = true;

    std::thread(&Client::listenerThread, this).detach();
}

void Client::handleServerRFS(const sf::IpAddress &ip, const unsigned short &port)
{
    std::cerr << ("[ Client::connectorThread ] -> Connection with server " + ip.toString() + ":" +
                  std::to_string(port) + " was refused.\n");

    connected = false;
}

void Client::handleServerBadResponse(const sf::IpAddress &ip, const unsigned short &port)
{
    std::cerr << ("[ Client::connectorThread ] -> Bad response from server " + ip.toString() + ":" +
                  std::to_string(port) + ", refusing to connect.\n");

    connected = false;
}

void Client::listenerThread()
{
    while (connected)
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

                    if (ip != serverIp || portBuffer != serverPort)
                    {
                        mutex.unlock();
                        continue; // Ignore any other connection that is not with server
                    }

                    std::string header;
                    packetBuffer >> header;

                    if (header == "KIL") // Handle player disconnection
                    {
                        disconnect();
                        mutex.unlock();
                        continue;
                    }
                    else if (header == "FILE")
                    {
                        receiveFile("Assets/Client/");
                    }
                }
            }
        }
        mutex.unlock();
    }
}

Client::Client() : serverIp(0, 0, 0, 0), serverPort(0), myUid(0), ready(true), connected(false)
{
    socket.setBlocking(false);

    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
        throw std::runtime_error("[ Client::Client ] -> Could not bind to a port\n");

    socketSelector.add(socket);
}

Client::~Client()
{
}

void Client::connect(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    if (connected)
        throw std::runtime_error("[ Client::connect ] -> Already has a active connection with " + serverIp.toString() +
                                 ":" + std::to_string(serverPort) + "\n");

    std::thread(&Client::connectorThread, this, ip, port, timeout).detach();
}

void Client::disconnect()
{
    if (!connected)
    {
        std::cerr << "[ Client::disconnect ] -> Not connected to any server." << "\n";
        return;
    }

    packetBuffer.clear();
    packetBuffer << "KIL";

    if (!send(packetBuffer))
        std::cerr << "[ Client::disconnect ] -> Could not communicate with server. Disconnecting anyways." << "\n";

    connected = false;

    std::cout << "[ Client::disconnect ] -> Disconnected from server " << serverIp.toString() << ":" << serverPort
              << ".\n";

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
        std::cerr << "[ Client::send ] -> Error sending packet to " << serverIp.toString() << ":"
                  << std::to_string(serverPort) << "\n";

        return false;
    }

    packet.clear();

    return true;
}

const bool Client::send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port)
{
    if (socket.send(packet, ip, port) != sf::Socket::Status::Done)
    {
        std::cerr << "[ Client::send ] -> Error sending packet to " << serverIp.toString() << ":"
                  << std::to_string(serverPort) << "\n";

        return false;
    }

    return true;
}

void Client::sendFile(std::filesystem::path path, std::ios::openmode mode)
{
    if (!std::filesystem::exists(path))
        throw std::runtime_error("[ Client::sendFile ] -> File \"" + path.string() = "\" does not exist.\n");

    std::ifstream file(path, mode);

    if (!file.is_open())
        throw std::runtime_error("[ Client::sendFile ] -> Could not open file \"" + path.string() = "\"\n");

    FileDescriptor f_desc;

    f_desc.filename = path.filename().string();
    f_desc.filesize = std::filesystem::file_size(path);
    f_desc.mode = static_cast<int>(mode);
    f_desc.part = 1;
    f_desc.total_parts = 1;

    if (f_desc.filesize > sf::UdpSocket::MaxDatagramSize) // TODO: Segment files bigger than MaxDatagramSize
        throw std::runtime_error("[ Client::sendFile ] -> File \"" + path.string() =
                                     "\" too big (" + std::to_string(f_desc.filesize) + " bytes)\n");

    packetBuffer.clear();
    packetBuffer << "FILE" << myUid;
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

    if (send(packetBuffer))
        std::cout << "[ Client::sendFile ] -> Sent file \"" << path.string() << "\" (" << f_desc.filesize << " bytes)"
                  << " to " << serverIp.toString() << ":" << serverPort << "\n";
}

void Client::receiveFile(std::filesystem::path folder)
{
    if (!std::filesystem::exists(folder))
        std::filesystem::create_directory(folder);

    std::string path_str = folder.string();
    if (path_str.back() != '/')
        path_str.push_back('/');

    FileDescriptor f_desc;
    packetBuffer >> f_desc;

    // std::cout << f_desc.filename << " " << f_desc.filesize << "\n";

    std::ofstream file(path_str + f_desc.filename, static_cast<std::ios::openmode>(f_desc.mode));

    if (!file.is_open())
        throw std::runtime_error("[ Client::receiveFile ] -> Could not write file \"" + path_str + f_desc.filename +
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

    file.close();
    packetBuffer.clear();

    std::cout << "[ Client::receiveFile ] -> Received file \"" << folder.string() + f_desc.filename << "\" ("
              << f_desc.filesize << " bytes)" << " to " << serverIp.toString() << ":" << serverPort << "\n";
}
