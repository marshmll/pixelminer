/**
 * @file PacketAddress.hxx
 * @brief Contains the `PacketAddress` structure, which represents an address for a network packet.
 */

#pragma once

#include <SFML/Network.hpp>

/**
 * @struct PacketAddress
 * @brief Represents the IP address and port of a packet destination or source.
 *
 * This structure encapsulates the address information required for sending or receiving network packets.
 * It holds both the IP address and port number to define a specific network endpoint.
 */
struct PacketAddress
{
    /**
     * @brief The IP address of the packet's destination or source.
     *
     * Default initialized to an invalid IP address (0.0.0.0).
     */
    sf::IpAddress ip = sf::IpAddress(0, 0, 0, 0);

    /**
     * @brief The port number associated with the packet's destination or source.
     *
     * Default initialized to 0.
     */
    unsigned short port = 0;
};
