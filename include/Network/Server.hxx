#pragma once

#include "Network/File.hxx"
#include "Network/PacketAddress.hxx"
#include "Tools/Logger.hxx"

struct Connection
{
    sf::IpAddress ip = sf::IpAddress(0, 0, 0, 0);
    unsigned short port = 0;
    float timeout = 10.f;
    sf::Clock timeoutClock;
    bool active = true;
};

class Server
{
  private:
    std::string myUuid;

    Logger logger;

    std::mutex mutex;

    sf::SocketSelector socketSelector;
    sf::UdpSocket socket;

    std::map<std::string, Connection> connections;
    std::queue<std::pair<PacketAddress, sf::Packet>> packetQueue;

    std::atomic_bool online;

    sf::Packet pktBuf;
    sf::IpAddress ipBuf;
    unsigned short portBuf;

    void listenerThread();

    void handler();

    void handleTimedOutConnections();

    void handleAskUuid(const std::string &uuid, const sf::IpAddress &ip, const unsigned short &port);

    void setOnline(const bool &online);

  public:
    Server(const std::string &uuid);

    ~Server();

    void listen(const unsigned short port);

    const bool createConnection(const sf::IpAddress &ip, const unsigned short &port, const std::string &uuid);

    void disconnectClient(const std::string &uuid);

    const bool isClientConnected(const sf::IpAddress &ip, const unsigned short &port);

    const bool send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port);

    void sendControlMessage(const std::string header, const sf::IpAddress &ip, const unsigned short &port);

    void sendFile(const sf::IpAddress &ip, const unsigned short &port, std::filesystem::path path,
                  std::ios::openmode mode);

    void receiveFile(const sf::IpAddress &ip, const unsigned short &port, std::filesystem::path folder);

    void shutdown();

    std::optional<std::pair<PacketAddress, sf::Packet>> consumePacket();
};