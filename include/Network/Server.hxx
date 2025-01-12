#pragma once

#include "Network/File.hxx"

class Server
{
  private:
    std::mutex mutex;

    sf::SocketSelector socketSelector;
    sf::UdpSocket socket;

    std::map<sf::IpAddress, unsigned short> clients;
    std::queue<sf::Packet> packetQueue;

    bool online;

    sf::Packet packetBuffer;
    sf::IpAddress ipBuffer;
    unsigned short portBuffer;

    void listenerThread();

  public:
    Server();

    ~Server();

    void listen(const unsigned short port);

    const bool connectClient(const sf::IpAddress &ip, const unsigned short &port);

    void disconnectClient(const sf::IpAddress &ip);

    void sendControlMessage(const std::string header, const sf::IpAddress &ip, const unsigned short &port);

    void sendFile(const sf::IpAddress client, std::filesystem::path path, std::ios::openmode mode);

    void receiveFile(const sf::IpAddress client, std::filesystem::path folder);

    void shutdown();

    std::optional<sf::Packet> consumePacket();
};