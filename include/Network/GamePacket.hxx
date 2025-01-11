#pragma once

#include "Network/GameData.hxx"
#include <SFML/Network/Packet.hpp>

struct GamePacket
{
    std::string header;
    sf::Packet data;
};

sf::Packet &operator>>(sf::Packet &packet, GamePacket &game_packet);

sf::Packet &operator<<(sf::Packet &packet, GamePacket &game_packet);