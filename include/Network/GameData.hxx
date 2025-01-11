#pragma once

#include "Entities/Playable/Player.hxx"

struct EntityData
{
    std::string name;
    std::uint64_t id;
    float pos_x;
    float pos_y;
    unsigned int mov_state;
    unsigned int mov_direction;
};

struct PlayerData : EntityData
{
};