#pragma once

#include "Engine/Configuration.hxx"
#include "stdafx.hxx"

enum TileId : std::uint32_t
{
    Unknown = 0,
    Dirt,
};

typedef struct
{
    std::string name;
    std::uint32_t id;
    sf::IntRect textureRect;
} TileData;