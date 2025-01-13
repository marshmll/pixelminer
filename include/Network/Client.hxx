#pragma once

#include "Network/File.hxx"
#include "Tools/Logger.hxx"

using ConnectionUID = std::uint32_t;

class Client
{
  private:
    Logger logger;

    std::mutex mutex;

    sf::SocketSelector socketSelector;
    sf::UdpSocket socket;

    sf::IpAddress serverIp;
    unsigned short serverPort;
    ConnectionUID myUid;

    std::queue<sf::Packet> packetQueue;

    bool ready;
    bool connected;

    sf::Packet packetBuffer;

    std::optional<sf::IpAddress> ipBuffer;
    unsigned short portBuffer;

    void connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout);

    void listenerThread();

    void handleServerACKUID(const sf::IpAddress &ip, const unsigned short &port);

    void handleServerRFS(const sf::IpAddress &ip, const unsigned short &port);

    void handleServerBadResponse(const sf::IpAddress &ip, const unsigned short &port);

    void setConnected(const bool &connected);

    void setReady(const bool &ready);

  public:
    Client();
    ~Client();

    void connect(const sf::IpAddress &ip, const unsigned short &port, const float &timeout = 10.f);

    void disconnect();

    const bool isReady() const;

    const bool isConnected() const;

    const bool send(sf::Packet &packet);

    const bool send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port);

    void sendFile(const std::filesystem::path &path, std::ios::openmode &mode);

    void receiveFile(const std::filesystem::path &folder);
};