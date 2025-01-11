#pragma once

#include "Network/GameData.hxx"
#include <SFML/Network/Packet.hpp>

enum ControlPacketType : unsigned int
{
    Connect,
    Acknowledge,
    Refuse,
    Disconnect
};

struct GamePacket
{
    unsigned int header;
    sf::Packet data;
};

sf::Packet &operator>>(sf::Packet &packet, GamePacket &game_packet);

sf::Packet &operator<<(sf::Packet &packet, GamePacket &game_packet);