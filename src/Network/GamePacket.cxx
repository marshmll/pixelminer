#include "Network/GamePacket.hxx"
#include "stdafx.hxx"

sf::Packet &operator>>(sf::Packet &packet, GamePacket &game_packet)
{
    packet >> game_packet.type;
    game_packet.data = packet;
    return packet;
}

sf::Packet &operator<<(sf::Packet &packet, GamePacket &game_packet)
{
    packet << game_packet.type;
    packet.append(packet.getData(), packet.getDataSize());
    return packet;
}