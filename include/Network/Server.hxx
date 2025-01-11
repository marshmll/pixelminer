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
    sf::IpAddress ipBuffer;
    unsigned short portBuffer;

    void listenerThread();

    const bool subscribeClient(const sf::IpAddress &ip, const unsigned short &port);

    void unsubscribeClient(const sf::IpAddress &ip);

    void sendControlMessage(const std::string header, const sf::IpAddress &ip, const unsigned short &port);

  public:
    Server();

    ~Server();

    void listen(const unsigned short port);

    void shutdown();

    std::optional<GamePacket> consumePacket();
};