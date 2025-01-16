#pragma once

#include "Engine/Configuration.hxx"
#include "stdafx.hxx"

enum TileId : std::uint32_t
{
    Unknown = 0,
    Dirt,
    Stone,
    Grass,
    GrassSide,
    Cobblestone,
    Sand,
    Snow,
    Water,
    GrassTopFront,
    GrassBottomFront,
    GrassBottomBack,
    GrassCurveTopFront,
    GrassCurveBottomFront,
    GrassCurveBottomBack,
    GrassTop,
    GrassBottom,
    GrassFront,
};

typedef struct
{
    std::string name;
    std::uint32_t id;
    sf::IntRect textureRect;
} TileData;