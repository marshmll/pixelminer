#pragma once

#include "Network/GamePacket.hxx"

using NetworkAddress = std::pair<sf::IpAddress, unsigned short>;

class Client
{
  private:
    std::mutex mutex;

    sf::SocketSelector socketSelector;
    sf::UdpSocket clientSocket;

    std::optional<NetworkAddress> serverAddress;

    std::queue<sf::Packet> packetQueue;

    bool ready;
    bool connected;

    sf::Packet packetBuffer;
    GamePacket gamePacketBuffer;

    std::optional<sf::IpAddress> ipBuffer;
    unsigned short portBuffer;

    void connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout);

    void listenerThread();

  public:
    Client();
    ~Client();

    void connect(const sf::IpAddress &ip, const unsigned short &port, const float &timeout = 10.f);

    const bool isReady() const;

    const bool isConnected() const;
};