#pragma once

#include "Network/File.hxx"
#include "Tools/Logger.hxx"

struct Connection
{
    sf::IpAddress ip = sf::IpAddress(0, 0, 0, 0);
    unsigned short port = 0;
    float timeout = 10.f;
    sf::Clock timeoutClock;
    bool active = true;
};

using ConnectionUID = std::uint32_t;

class Server
{
  private:
    Logger logger;

    std::mutex mutex;

    sf::SocketSelector socketSelector;
    sf::UdpSocket socket;

    std::map<ConnectionUID, Connection> connections;
    std::queue<sf::Packet> packetQueue;

    bool online;

    sf::Packet pktBuf;
    sf::IpAddress ipBuf;
    unsigned short portBuf;

    void listenerThread();

    void handleTimedOutConnections();

    void handleAsk(const sf::IpAddress &ip, const unsigned short &port);

    void handleUidAck(const ConnectionUID &uid);

    const ConnectionUID generateConnectionUID();

  public:
    Server();

    ~Server();

    void listen(const unsigned short port);

    const bool createConnection(const sf::IpAddress &ip, const unsigned short &port, const ConnectionUID &uid);

    void disconnectClient(const sf::IpAddress &ip);

    const bool isClientConnected(const sf::IpAddress &ip, const unsigned short &port);

    const bool send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port);

    void sendControlMessage(const std::string header, const sf::IpAddress &ip, const unsigned short &port);

    void sendFile(const sf::IpAddress &ip, const unsigned short &port, std::filesystem::path path,
                  std::ios::openmode mode);

    void receiveFile(const sf::IpAddress &ip, const unsigned short &port, std::filesystem::path folder);

    void shutdown();

    std::optional<sf::Packet> consumePacket();
};