#pragma once

#include "Network/File.hxx"

class Client
{
  private:
    std::mutex mutex;

    sf::SocketSelector socketSelector;
    sf::UdpSocket socket;

    sf::IpAddress serverIp;
    unsigned short serverPort;

    std::queue<sf::Packet> packetQueue;

    bool ready;
    bool connected;

    sf::Packet packetBuffer;

    std::optional<sf::IpAddress> ipBuffer;
    unsigned short portBuffer;

    void connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout);

    void handleServerACK(const sf::IpAddress &ip, const unsigned short &port);

    void handleServerRFS(const sf::IpAddress &ip, const unsigned short &port);

    void handleServerBadResponse(const sf::IpAddress &ip, const unsigned short &port);

    void listenerThread();

  public:
    Client();
    ~Client();

    void connect(const sf::IpAddress &ip, const unsigned short &port, const float &timeout = 10.f);

    void disconnect();

    const bool isReady() const;

    const bool isConnected() const;

    void sendFile(std::filesystem::path path, std::ios::openmode mode);

    void receiveFile(std::filesystem::path folder);
};