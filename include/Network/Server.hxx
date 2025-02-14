/**
 * @file Server.hxx
 * @brief Contains the `Server` class and related structures for handling server-side network operations.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "Network/File.hxx"
#include "Network/PacketAddress.hxx"
#include "Tools/JSON.hxx"
#include "Tools/Logger.hxx"

/**
 * @struct Connection
 * @brief Represents a connection between the server and a client.
 *
 * This structure holds information about an active client connection, including the client's IP address,
 * port number, the timeout duration, a clock to track the timeout, and whether the connection is active.
 */
struct Connection
{
    /**
     * @brief The IP address of the client.
     *
     * Default initialized to an invalid IP address (0.0.0.0).
     */
    sf::IpAddress ip = sf::IpAddress(0, 0, 0, 0);

    /**
     * @brief The port number used by the client.
     *
     * Default initialized to 0.
     */
    unsigned short port = 0;

    /**
     * @brief The timeout duration for the connection.
     *
     * Default set to 10 seconds.
     */
    float timeout = 10.f;

    /**
     * @brief The clock used to track the elapsed time for the connection timeout.
     */
    sf::Clock timeoutClock;

    /**
     * @brief Indicates whether the connection is active.
     *
     * Default set to true.
     */
    bool active = true;
};

/**
 * @class Server
 * @brief A class representing the server handling client connections and communication.
 *
 * The `Server` class provides functionality for handling network communication, managing client connections,
 * sending and receiving files, and managing packet queues. It uses UDP sockets and can handle multiple clients
 * concurrently.
 */
class Server
{
  private:
    std::string myUuid;                                           ///< The server's unique identifier (UUID).
    Logger logger;                                                ///< Logger instance for logging server activity.
    std::mutex mutex;                                             ///< Mutex for thread synchronization.
    sf::SocketSelector socketSelector;                            ///< Selector used to monitor multiple sockets.
    sf::UdpSocket socket;                                         ///< The UDP socket used by the server.
    std::unordered_map<std::string, Connection> connections;      ///< A map of connected clients by UUID.
    unsigned int maxConnections;                                  ///< Maximum of connections accepted.
    std::queue<std::pair<PacketAddress, sf::Packet>> packetQueue; ///< A queue for received packets.
    std::atomic_bool online;                                      ///< Flag indicating whether the server is online.

    /**
     * @brief Listens for incoming packets and handles client connections.
     */
    void listenerThread();

    /**
     * @brief Handles incoming packets and processes them accordingly.
     */
    void handler();

    /**
     * @brief Checks for timed-out connections and disconnects them.
     */
    void handleTimedOutConnections();

    /**
     * @brief Handles a request for a UUID from a client.
     * @param uuid The UUID of the client.
     * @param ip The IP address of the client.
     * @param port The port number of the client.
     */
    void handleAskUuid(const std::string &uuid, const sf::IpAddress &ip, unsigned short port);

    void sendServerInfo(const sf::IpAddress &ip, unsigned short port);

    /**
     * @brief Sets the server's online status.
     * @param online Boolean indicating whether the server should be online or offline.
     */
    void setOnline(bool online);

  public:
    /**
     * @brief Constructor for the `Server` class.
     * @param uuid The server's unique identifier (UUID).
     */
    Server(const std::string &uuid);

    /**
     * @brief Destructor for the `Server` class.
     */
    ~Server();

    /**
     * @brief Starts listening for incoming connections on the specified port.
     * @param port The port to bind the server socket to.
     */
    void listen(unsigned short port);

    /**
     * @brief Creates a connection with a client.
     * @param ip The IP address of the client.
     * @param port The port number of the client.
     * @param uuid The UUID of the client.
     * @return true if the connection is successfully created, false otherwise.
     */
    bool createConnection(const sf::IpAddress &ip, unsigned short port, const std::string &uuid);

    /**
     * @brief Disconnects a client by their UUID.
     * @param uuid The UUID of the client to disconnect.
     */
    void disconnectClient(const std::string &uuid);

    /**
     * @brief Checks if a client is connected using the specified IP and port.
     * @param ip The IP address of the client.
     * @param port The port number of the client.
     * @return true if the client is connected, false otherwise.
     */
    bool isClientConnected(const sf::IpAddress &ip, unsigned short port) const;

    /**
     * @brief Sends a packet to a client.
     * @param packet The packet to send.
     * @param ip The IP address of the client.
     * @param port The port number of the client.
     * @return true if the packet was successfully sent, false otherwise.
     */
    bool send(sf::Packet &packet, const sf::IpAddress &ip, unsigned short port);

    /**
     * @brief Sends a control message to a client.
     * @param header The control message header.
     * @param ip The IP address of the client.
     * @param port The port number of the client.
     */
    void sendControlMessage(const std::string &header, const sf::IpAddress &ip, unsigned short port);

    /**
     * @brief Sends a file to a client.
     * @param ip The IP address of the client.
     * @param port The port number of the client.
     * @param path The path to the file to send.
     * @param mode The mode in which to open the file (e.g., binary or text).
     */
    void sendFile(const sf::IpAddress &ip, unsigned short port, const std::filesystem::path &path,
                  std::ios::openmode mode);

    /**
     * @brief Receives a file from a client and saves it to the specified folder.
     * @param ip The IP address of the client.
     * @param port The port number of the client.
     * @param folder The folder where the received file should be saved.
     * @param packet The packet containing the file data.
     */
    void receiveFile(const sf::IpAddress &ip, unsigned short port, const std::filesystem::path &folder,
                     sf::Packet &packet);

    /**
     * @brief Shuts down the server and disconnects all clients.
     */
    void shutdown();

    /**
     * @brief Consumes a packet from the packet queue, if available.
     * @return An optional containing the packet address and packet if available, or `std::nullopt` if the queue is
     * empty.
     */
    std::optional<std::pair<PacketAddress, sf::Packet>> consumePacket();
};
