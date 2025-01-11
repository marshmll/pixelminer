#pragma once

#include "Network/GamePacket.hxx"

class Server
{
  private:
    std::mutex mutex;

    sf::SocketSelector socketSelector;
    sf::UdpSocket serverSocket;

    std::map<sf::IpAddress, unsigned short> clients;
    std::queue<GamePacket> packetQueue;

    bool online;

    sf::Packet packetBuffer;
    std::optional<sf::IpAddress> ipBuffer;
    unsigned short portBuffer;

    void listenerThread();

  public:
    Server();

    ~Server();

    void listen(const unsigned short port);

    void shutdown();

    std::optional<GamePacket> consumePacket();
};