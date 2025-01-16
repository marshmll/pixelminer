#pragma once

#include "Network/File.hxx"
#include "Network/PacketAddress.hxx"
#include "Tools/Logger.hxx"

class Client
{
  private:
    std::string myUuid;

    Logger logger;

    std::mutex mutex;

    sf::SocketSelector socketSelector;
    sf::UdpSocket socket;

    sf::IpAddress serverIp;
    unsigned short serverPort;

    std::queue<std::pair<PacketAddress, sf::Packet>> packetQueue;

    bool ready;
    bool connected;

    sf::Packet pktBuf;

    std::optional<sf::IpAddress> ipBuffer;
    unsigned short portBuf;

    void connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout);

    void listenerThread();

    void handler();

    void handleServerAck(const sf::IpAddress &ip, const unsigned short &port);

    void handleServerRcn(const sf::IpAddress &ip, const unsigned short &port);

    void handleServerRfs(const sf::IpAddress &ip, const unsigned short &port);

    void handleServerBadResponse(const sf::IpAddress &ip, const unsigned short &port);

    void setConnected(const bool &connected);

    void setReady(const bool &ready);

  public:
    Client(const std::string &uuid);
    ~Client();

    void connect(const sf::IpAddress &ip, const unsigned short &port, const float &timeout = 10.f);

    void disconnect();

    const bool isReady() const;

    const bool isConnected() const;

    const bool send(sf::Packet &packet);

    const bool send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port);

    void sendFile(const std::filesystem::path &path, std::ios::openmode &mode);

    void receiveFile(const std::filesystem::path &folder);

    std::optional<std::pair<PacketAddress, sf::Packet>> consumePacket();
};