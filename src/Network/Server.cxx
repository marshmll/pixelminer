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

                    if (game_packet.header == "CON") // Handle player connection
                    {
                        if (!connectClient(ipBuffer, portBuffer))
                            sendControlMessage("RFS", ipBuffer, portBuffer);
                        else
                            sendControlMessage("ACK", ipBuffer, portBuffer);
                    }
                    else if (clients.count(ipBuffer) == 0)
                    {
                        std::cerr << "[ Server::listenerThread ] -> Client with IP " << ipBuffer.toString()
                                  << " is not connected."
                                  << "\n";

                        mutex.unlock();
                        continue; // Ignore clients that did not ask to connect
                    }
                    else if (game_packet.header == "KIL") // Handle player disconnection
                    {
                        disconnectClient(ipBuffer);
                    }
                }
            }
        }
        mutex.unlock();
    }
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
        GamePacket pkt = (GamePacket){header, {}};

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

Server::Server() : online(false), ipBuffer(0, 0, 0, 0)
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

    online = true;

    std::cout << "[ Server::listen ] -> Server (" << sf::IpAddress::getLocalAddress()->toString() << ":"
              << std::to_string(serverSocket.getLocalPort()) << ") online" << "\n";
}

void Server::shutdown()
{
    if (!online)
    {
        std::cerr << "[ Server::shutdown ] -> Server is not online" << "\n";
        return;
    }

    unsigned short port = serverSocket.getLocalPort();

    for (auto &[ip, port] : clients)
    {
        std::cout << "[ Server::shutdown ] -> Killing connection with " << ip.toString() << ":" << std::to_string(port)
                  << "\n";
        sendControlMessage("KIL", ip, port);
    }

    online = false;
    serverSocket.unbind();
    socketSelector.clear();

    std::cout << "[ Server::shutdown ] -> Server (" << sf::IpAddress::getLocalAddress()->toString() << ":"
              << std::to_string(port) << ") offline" << "\n";
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