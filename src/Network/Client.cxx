#include "Network/Client.hxx"
#include "stdafx.hxx"

void Client::connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    mutex.lock();
    clientSocket.setBlocking(true); // Set blocking while establishing connection.
    ready = false;

    sf::Packet data;
    GamePacket conn_pkt = (GamePacket){Connect, {}};

    packetBuffer << conn_pkt;

    if (clientSocket.send(packetBuffer, ip, port) != sf::Socket::Status::Done)
    {
        std::cerr << "[ Client ] -> Could not ask for a connection with " << ip.toString() << ":"
                  << std::to_string(port) << "\n";
        ready = true;
        connected = false;

        mutex.unlock();

        return;
    }

    if (socketSelector.wait(sf::seconds(timeout)))
    {
        if (socketSelector.isReady(clientSocket))
        {
            if (clientSocket.receive(packetBuffer, ipBuffer, portBuffer) == sf::Socket::Status::Done)
            {
                ready = true;
                packetBuffer >> gamePacketBuffer;

                std::cout << gamePacketBuffer.header << "\n";

                if (gamePacketBuffer.header == Acknowledge)
                {
                    std::cout << "[ Client ] -> Connected to server: " << ip.toString() << ":" << std::to_string(port)
                              << "\n";

                    connected = true;
                }
                else if (gamePacketBuffer.header == Refuse)
                {
                    std::cerr << ("[ Client ] -> Connection with server " + ip.toString() + ":" + std::to_string(port) +
                                  " refused.\n");

                    connected = false;
                }
                else
                {
                    std::cerr << ("[ Client ] -> Bad response from server " + ip.toString() + ":" +
                                  std::to_string(port) + ", refuse to connect.\n");

                    connected = false;
                }
            }
        }
    }
    else
    {
        std::cerr << ("[ Client ] -> Connection timeout with " + ip.toString() + ":" + std::to_string(port) + "\n");
        ready = true;
        connected = false;
    }

    mutex.unlock();
}

void Client::listenerThread()
{
    while (connected)
    {
        mutex.lock();
        if (socketSelector.wait(sf::seconds(10.f)))
        {
            if (socketSelector.isReady(clientSocket))
            {
                packetBuffer.clear();
                std::optional<sf::IpAddress> ip;

                if (clientSocket.receive(packetBuffer, ip, portBuffer) == sf::Socket::Status::Done)
                {
                    ipBuffer = ip.value();

                    if (ip != serverAddress->first || portBuffer != serverAddress->second)
                    {
                        mutex.unlock();
                        continue; // Ignore any other connection
                    }

                    GamePacket game_packet;
                    packetBuffer >> game_packet;

                    if (game_packet.header == Disconnect) // Handle player disconnection
                    {
                        std::cout << "[ Client ] -> Disconnected from server " << ip->toString() << ":"
                                  << clientSocket.getLocalPort() << "\n";
                        connected = false;
                        mutex.unlock();
                        return;
                    }
                }
            }
        }
        mutex.unlock();
    }
}

Client::Client() : ready(true), connected(false)
{
    clientSocket.setBlocking(false);

    if (clientSocket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
        throw std::runtime_error("[ Client ] -> Could not bind to a port\n");

    socketSelector.add(clientSocket);
}

Client::~Client()
{
}

void Client::connect(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    if (connected)
        throw std::runtime_error("[ Client::connect ] -> Already has a active connection with " +
                                 serverAddress->first.toString() + ":" + std::to_string(serverAddress->second) + "\n");

    std::thread(&Client::connectorThread, this, ip, port, timeout).detach();
}

const bool Client::isReady() const
{
    return ready;
}

const bool Client::isConnected() const
{
    return connected;
}
