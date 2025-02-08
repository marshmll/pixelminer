/**
 * @file Client.hxx
 * @brief Declares the Client class responsible for managing UDP-based client-server communication and file transfer.
 *
 * This file contains the declaration of the Client class, which allows a client to connect to a server,
 * send and receive packets, and transfer files. The client uses SFML for UDP communication and threading
 * to handle multiple tasks simultaneously.
 *
 * @note This header file should be paired with its corresponding implementation file (`Client.cxx`) for complete
 * functionality.
 */

#pragma once

#include "Network/File.hxx"
#include "Network/PacketAddress.hxx"
#include "Tools/Logger.hxx"

/**
 * @class Client
 * @brief Represents a UDP client that communicates with a server.
 *
 * The Client class provides methods for connecting to a server, sending/receiving packets,
 * and transferring files. It supports concurrent operations such as listening for incoming packets
 * and managing connection status in a separate thread.
 *
 * @note This class uses SFML's UDP socket for communication and threading for non-blocking operations.
 */
class Client
{
  private:
    /**
     * @brief A unique identifier for the client.
     */
    std::string myUuid;

    /**
     * @brief Logger for logging client activities.
     */
    Logger logger;

    /**
     * @brief Mutex for thread synchronization.
     */
    std::mutex mutex;

    /**
     * @brief Selector for monitoring multiple sockets for activity.
     */
    sf::SocketSelector socketSelector;

    /**
     * @brief The client's UDP socket for communication.
     */
    sf::UdpSocket socket;

    /**
     * @brief The IP address of the connected server.
     */
    sf::IpAddress serverIp;

    /**
     * @brief The port of the connected server.
     */
    unsigned short serverPort;

    /**
     * @brief Queue to hold received packets for processing.
     */
    std::queue<std::pair<PacketAddress, sf::Packet>> packetQueue;

    /**
     * @brief Flag indicating if the client is ready for communication.
     */
    bool ready;

    /**
     * @brief Flag indicating if the client is connected to the server.
     */
    bool connected;

    /**
     * @brief Temporary IP address buffer used during connection attempts.
     */
    std::optional<sf::IpAddress> ipBuffer;

    /**
     * @brief Temporary port buffer used during connection attempts.
     */
    unsigned short portBuf;

    /**
     * @brief Attempts to connect to a server in a separate thread.
     * @param ip The IP address of the server to connect to.
     * @param port The port of the server.
     * @param timeout The timeout duration in seconds for the connection attempt.
     */
    void connectorThread(const sf::IpAddress &ip, const unsigned short &port, const float &timeout);

    /**
     * @brief Listens for incoming packets from the server in a separate thread.
     */
    void listenerThread();

    /**
     * @brief Processes incoming packets and takes appropriate action.
     */
    void handler();

    /**
     * @brief Handles the "ACK" response from the server during connection.
     * @param ip The IP address of the server.
     * @param port The port of the server.
     */
    void handleServerAck(const sf::IpAddress &ip, const unsigned short &port);

    /**
     * @brief Handles the "RCN" (Reconnected) response from the server.
     * @param ip The IP address of the server.
     * @param port The port of the server.
     */
    void handleServerRcn(const sf::IpAddress &ip, const unsigned short &port);

    /**
     * @brief Handles the "RFS" (Connection Refused) response from the server.
     * @param ip The IP address of the server.
     * @param port The port of the server.
     */
    void handleServerRfs(const sf::IpAddress &ip, const unsigned short &port);

    /**
     * @brief Handles unexpected or invalid responses from the server.
     * @param ip The IP address of the server.
     * @param port The port of the server.
     */
    void handleServerBadResponse(const sf::IpAddress &ip, const unsigned short &port);

    /**
     * @brief Sets the connected status of the client.
     * @param connected A flag indicating whether the client is connected.
     */
    void setConnected(const bool &connected);

    /**
     * @brief Sets the ready status of the client.
     * @param ready A flag indicating whether the client is ready for communication.
     */
    void setReady(const bool &ready);

  public:
    /**
     * @brief Constructs a new Client object.
     * @param uuid The unique identifier for the client.
     */
    Client(const std::string &uuid);

    /**
     * @brief Destructor for the Client object.
     */
    ~Client();

    /**
     * @brief Initiates the connection process to the server.
     * @param ip The IP address of the server to connect to.
     * @param port The port of the server to connect to.
     * @param timeout The timeout duration in seconds for the connection attempt (default is 10 seconds).
     */
    void connect(const sf::IpAddress &ip, const unsigned short &port, const float &timeout = 10.f);

    /**
     * @brief Disconnects the client from the server.
     */
    void disconnect();

    /**
     * @brief Checks if the client is ready for communication.
     * @return True if the client is ready, otherwise false.
     */
    const bool isReady() const;

    /**
     * @brief Checks if the client is connected to a server.
     * @return True if the client is connected, otherwise false.
     */
    const bool isConnected() const;

    /**
     * @brief Sends a packet to the server.
     * @param packet The packet to be sent.
     * @return True if the packet was successfully sent, otherwise false.
     */
    const bool send(sf::Packet &packet);

    /**
     * @brief Sends a packet to a specified address and port.
     * @param packet The packet to be sent.
     * @param ip The IP address of the recipient.
     * @param port The port of the recipient.
     * @return True if the packet was successfully sent, otherwise false.
     */
    const bool send(sf::Packet &packet, const sf::IpAddress &ip, const unsigned short &port);

    /**
     * @brief Sends a file to the server.
     * @param path The file path to be sent.
     * @param mode The file open mode (e.g., `std::ios::binary`).
     */
    void sendFile(const std::filesystem::path &path, std::ios::openmode &mode);

    /**
     * @brief Receives a file from the server.
     * @param folder The destination folder where the file will be saved.
     * @param packet The packet containing the file data.
     */
    void receiveFile(const std::filesystem::path &folder, sf::Packet &packet);

    /**
     * @brief Consumes and returns a received packet from the packet queue.
     * @return An optional pair of packet address and packet data, or std::nullopt if no packets are available.
     */
    std::optional<std::pair<PacketAddress, sf::Packet>> consumePacket();
};
