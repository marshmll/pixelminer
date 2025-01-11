#include "Network/GamePacket.hxx"
#include "stdafx.hxx"

sf::Packet &operator>>(sf::Packet &packet, GamePacket &game_packet)
{
    packet >> game_packet.header;
    game_packet.data.append(packet.getData(), packet.getDataSize());
    return packet;
}

sf::Packet &operator<<(sf::Packet &packet, GamePacket &game_packet)
{
    packet << game_packet.header;
    packet.append(game_packet.data.getData(), game_packet.data.getDataSize());
    return packet;
}