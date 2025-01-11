#include "Network/Server.hxx"
#include "stdafx.hxx"

void Server::listenerThread()
{
    while (online)
    {
        mutex.lock();
        if (socketSelector.wait(sf::seconds(10.f)))
        {
            if (socketSelector.isReady(serverSocket))
            {
                packetBuffer.clear();
                std::optional<sf::IpAddress> ip;

                if (serverSocket.receive(packetBuffer, ip, portBuffer) == sf::Socket::Status::Done)
                {
                    ipBuffer = ip.value();

                    GamePacket game_packet;
                    packetBuffer >> game_packet;

                    if (game_packet.header == Connect) // Handle player connection
                    {
                        if (!subscribeClient(ipBuffer, portBuffer))
                            sendControlMessage(Refuse, ipBuffer, portBuffer);
                        else
                            sendControlMessage(Acknowledge, ipBuffer, portBuffer);
                    }
                    else if (clients.count(ipBuffer) == 0)
                    {
                        mutex.unlock();
                        continue; // Ignore clients that did not ask to connect
                    }
                    else if (game_packet.header == Disconnect) // Handle player disconnection
                    {
                        unsubscribeClient(ipBuffer);
                    }
                }
            }
        }
        mutex.unlock();
    }
}

const bool Server::subscribeClient(const sf::IpAddress &ip, const unsigned short &port)
{
    if (clients.count(ip) != 0)
    {
        std::cerr << "[ Server::subscribeClient ] -> Client with IP " << ip.toString() << " already subscribed."
                  << "\n";
        return false;
    }
    else
    {
        clients[ip] = port; // Link IP and Port
        return true;
    }
}

void Server::unsubscribeClient(const sf::IpAddress &ip)
{
    try
    {
        clients.erase(ip);
    }
    catch (std::exception e)
    {
        std::cerr << "[ Server::unsubscribeClient ] -> Client with IP " << ip.toString() << " is not subscribed."
                  << "\n";
    }
}

void Server::sendControlMessage(ControlPacketType type, const sf::IpAddress &ip, const unsigned short &port)
{
    try
    {
        GamePacket pkt = (GamePacket){Acknowledge, {}};

        packetBuffer.clear();
        packetBuffer << pkt;

        if (serverSocket.send(packetBuffer, ip, port) != sf::Socket::Status::Done)
        {
            std::cerr << "[ Server::sendControlMessage ] -> Could not send control message to " << ipBuffer.toString()
                      << ":" << std::to_string(portBuffer) << "\n";
        }
    }
    catch (std::exception e)
    {
        std::cerr << "[ Server::sendControlMessage ] -> Error while sending control message to " << ip.toString()
                  << ": " << e.what() << "\n";
    }
}

Server::Server() : online(true), ipBuffer(0, 0, 0, 0)
{
}

Server::~Server()
{
}

void Server::listen(const unsigned short port)
{
    serverSocket.setBlocking(false);

    if (serverSocket.bind(port) != sf::Socket::Status::Done)
        throw std::runtime_error("[ Server::listen ] -> Could not bind to port " + std::to_string(port) + "\n");

    socketSelector.add(serverSocket);

    std::thread(&Server::listenerThread, this).detach();

    std::cout << "[ Server::listen ] -> Server (" << sf::IpAddress::getLocalAddress()->toString() << ":"
              << std::to_string(serverSocket.getLocalPort()) << ") online" << "\n";
}

void Server::shutdown()
{
    if (!online)
    {
        std::cerr << "[ Server::shutdown ] -> Server is not online" << "\n";
    }
    std::cout << "[ Server::shutdown ] -> Server (" << sf::IpAddress::getLocalAddress()->toString() << ":"
              << std::to_string(serverSocket.getLocalPort()) << ") offline" << "\n";

    for (auto &[ip, port] : clients)
        sendControlMessage(Disconnect, ip, port);

    online = false;
    serverSocket.unbind();
    socketSelector.clear();
}

std::optional<GamePacket> Server::consumePacket()
{
    std::optional<GamePacket> packet;

    if (!packetQueue.empty())
    {
        packet.emplace(packetQueue.front());
        packetQueue.pop();
    }

    return packet;
}