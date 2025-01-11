#pragma once

#include "Network/GameData.hxx"
#include <SFML/Network/Packet.hpp>

enum PacketType : unsigned int
{
    Connect,
    Accept,
    Refuse,
    EntityData,
    PlayerData,
};

struct GamePacket
{
    unsigned int type;
    sf::Packet data;
};

sf::Packet &operator>>(sf::Packet &packet, GamePacket &game_packet);

sf::Packet &operator<<(sf::Packet &packet, GamePacket &game_packet);