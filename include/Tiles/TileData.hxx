#pragma once

#include "Engine/Configuration.hxx"
#include "stdafx.hxx"

enum TileId : std::uint32_t
{
    Unknown = 0,
    Dirt,
    Stone,
    GrassSide,
    Cobblestone,
    GrassTop,
    Sand,
    Snow,
    Water,
    GrassTopFront,
    GrassFront,
};

typedef struct
{
    std::string name;
    std::uint32_t id;
    sf::IntRect textureRect;
} TileData;