#pragma once

#include "Engine/Configuration.hxx"
#include "stdafx.hxx"

typedef struct
{
    std::string id;
    std::string name;
    sf::IntRect rect;
    int size;
} TileData;