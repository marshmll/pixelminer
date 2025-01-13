#include "Network/Client.hxx"
#include "stdafx.hxx"
#include <filesystem>
#include <fstream>
#include <optional>

void Client::connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    std::lock_guard<std::mutex> lock(mutex);
    socket.setBlocking(true); // Blocking mode for connection
    setReady(false);

    packetBuffer.clear();
    packetBuffer << "ASK";

    if (!send(packetBuffer, ip, port))
    {
        logger.logError("Could not connect to " + ip.toString() + ":" + std::to_string(port) + "");
        setReady(true);
        setConnected(false);
        return;
    }

    if (socketSelector.wait(sf::seconds(timeout)) && socketSelector.isReady(socket))
    {
        if (socket.receive(packetBuffer, ipBuffer, portBuffer) == sf::Socket::Status::Done)
        {
            setReady(true);
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
    else
    {
        logger.logError("Connection timeout: " + ip.toString() + ":" + std::to_string(port));
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
            packetBuffer.clear();
            std::optional<sf::IpAddress> ip;

            if (socket.receive(packetBuffer, ip, portBuffer) == sf::Socket::Status::Done)
            {
                if (ip != serverIp || portBuffer != serverPort)
                    continue;

                std::string header;
                packetBuffer >> header;

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
}

void Client::handleServerACKUID(const sf::IpAddress &ip, const unsigned short &port)
{
    logger.logInfo("Connected to server: " + ip.toString() + ":" + std::to_string(port) + "");
    serverIp = ip;
    serverPort = port;

    packetBuffer.clear();
    packetBuffer << "UID+ACK" << myUid;

    send(packetBuffer);
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

    packetBuffer.clear();
    packetBuffer << "KIL";

    if (!send(packetBuffer))
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

    packetBuffer.clear();
    packetBuffer << "FILE" << myUid << fd;

    char c;
    while (file.get(c))
        packetBuffer << c;

    file.close();

    if (send(packetBuffer))
        logger.logInfo("Sent file \"" + path.string() + "\" (" + std::to_string(fd.filesize) + " bytes) to " +
                       serverIp.toString());
}

void Client::receiveFile(const std::filesystem::path &folder)
{
    if (!File::validatePath(folder))
        std::filesystem::create_directory(folder);

    File::FileDescriptor fd;
    packetBuffer >> fd;

    std::ofstream file(folder / fd.filename, static_cast<std::ios::openmode>(fd.mode));
    if (!file.is_open())
        logger.logError("Could not write file \"" + (folder / fd.filename).string() + "\"");

    std::uint8_t c;
    while (packetBuffer >> c)
        file.put(c);

    file.close();
    logger.logInfo("Received file \"" + (folder / fd.filename).string() + "\" (" + std::to_string(fd.filesize) +
                   " bytes).");
}
