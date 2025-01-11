#include "Network/Client.hxx"
#include "stdafx.hxx"

void Client::connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout)
{
    mutex.lock();
    clientSocket.setBlocking(true); // Set blocking while establishing connection.
    ready = false;

    sf::Packet data;
    GamePacket conn_pkt = (GamePacket){"CON", {}};

    packetBuffer << conn_pkt;

    if (clientSocket.send(packetBuffer, ip, port) != sf::Socket::Status::Done)
    {
        std::cerr << "[ Client::connectorThread ] -> Could not ask for a connection with " << ip.toString() << ":"
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
                GamePacket game_packet;
                packetBuffer >> game_packet;

                std::cout << "Received: " << game_packet.header << "\n";

                if (game_packet.header == "ACK")
                {
                    std::cout << "[ Client::connectorThread ] -> Connected to server: " << ip.toString() << ":"
                              << std::to_string(port) << "\n";

                    connected = true;
                    serverAddress->first = ipBuffer.value();
                    serverAddress->second = portBuffer;

                    std::thread(&Client::listenerThread, this).detach();
                }
                else if (game_packet.header == "RFS")
                {
                    std::cerr << ("[ Client::connectorThread ] -> Connection with server " + ip.toString() + ":" +
                                  std::to_string(port) + " was refused.\n");

                    connected = false;
                }
                else
                {
                    std::cerr << ("[ Client::connectorThread ] -> Bad response from server " + ip.toString() + ":" +
                                  std::to_string(port) + ", refusing to connect.\n");

                    connected = false;
                }
            }
        }
    }
    else
    {
        std::cerr << ("[ Client::connectorThread ] -> Connection timeout with " + ip.toString() + ":" +
                      std::to_string(port) + "\n");
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

                    // if (ip != serverAddress->first || portBuffer != serverAddress->second)
                    // {
                    //     mutex.unlock();
                    //     continue; // Ignore any other connection
                    // }

                    GamePacket game_packet;
                    packetBuffer >> game_packet;

                    std::cout << game_packet.header << "\n";

                    if (game_packet.header == "KIL") // Handle player disconnection
                    {
                        std::cout << "[ Client::listenerThread ] -> Disconnected from server " << ip->toString() << ":"
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
        throw std::runtime_error("[ Client::Client ] -> Could not bind to a port\n");

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
