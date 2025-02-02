#pragma once

#include "Engine/Configuration.hxx"
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

    std::unordered_map<std::string, Connection> connections;
    std::queue<std::pair<PacketAddress, sf::Packet>> packetQueue;

    std::atomic_bool online;

    void listenerThread();

    void handler();

    void handleTimedOutConnections();

    void handleAskUuid(const std::string &uuid, const sf::IpAddress &ip, unsigned short port);

    void setOnline(bool online);

  public:
    Server(const std::string &uuid);

    ~Server();

    void listen(unsigned short port);

    bool createConnection(const sf::IpAddress &ip, unsigned short port, const std::string &uuid);

    void disconnectClient(const std::string &uuid);

    bool isClientConnected(const sf::IpAddress &ip, unsigned short port) const;

    bool send(sf::Packet &packet, const sf::IpAddress &ip, unsigned short port);

    void sendControlMessage(const std::string &header, const sf::IpAddress &ip, unsigned short port);

    void sendFile(const sf::IpAddress &ip, unsigned short port, const std::filesystem::path &path,
                  std::ios::openmode mode);

    void receiveFile(const sf::IpAddress &ip, unsigned short port, const std::filesystem::path &folder,
                     sf::Packet &packet);

    void shutdown();

    std::optional<std::pair<PacketAddress, sf::Packet>> consumePacket();
};