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
                if (serverSocket.receive(packetBuffer, ipBuffer, portBuffer) == sf::Socket::Status::Done)
                {
                    GamePacket game_packet;
                    packetBuffer >> game_packet;

                    if (game_packet.type == PacketType::Connect) // Handle player connection
                    {
                        clients[ipBuffer.value()] = portBuffer; // Link IP and Port

                        GamePacket cnfrm_pkt = (GamePacket){PacketType::Accept, {}};
                        packetBuffer << cnfrm_pkt;

                        if (serverSocket.send(packetBuffer, ipBuffer.value(), portBuffer) != sf::Socket::Status::Done)
                        {
                            std::cerr << "[ Server ] -> Error while accepting connection from client "
                                      << ipBuffer.value().toString() << ":" << std::to_string(portBuffer) << "\n";

                            clients.erase(ipBuffer.value());
                        }

                        std::cout << "[ Server ] -> Connection with " << ipBuffer.value().toString() << ":"
                                  << std::to_string(portBuffer) << " established successfully" << "\n";
                    }
                }
            }
        }
        mutex.unlock();
    }
}

Server::Server() : online(true)
{
}

Server::~Server()
{
}

void Server::listen(const unsigned short port)
{
    serverSocket.setBlocking(false);

    if (serverSocket.bind(port) != sf::Socket::Status::Done)
        throw std::runtime_error("[ Server ] -> Could not bind to port " + std::to_string(port) + "\n");

    socketSelector.add(serverSocket);

    std::thread(&Server::listenerThread, this).detach();
}

void Server::shutdown()
{
    online = false;
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