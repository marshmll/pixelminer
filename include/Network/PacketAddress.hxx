#pragma once

struct PacketAddress
{
    sf::IpAddress ip = sf::IpAddress(0, 0, 0, 0);
    unsigned short port = 0;
};