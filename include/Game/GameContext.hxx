/**
 * @file GameContext.hxx
 * @brief Declares the GameContext struct.
 */

#pragma once

#include "Entities/Playable/Player.hxx"
#include "Map/Map.hxx"
#include "States/State.hxx"

/**
 * @struct GameContext
 * @brief Aggregates a series of attributes of the GameState class used for commands interpreting.
 */
struct GameContext
{
    State *currentState;                                 ///< A raw pointer to the current GameState.
    std::vector<std::shared_ptr<Entity>> globalEntities; ///< List of all non-player entities in the game world.
    std::unordered_map<std::string, std::shared_ptr<Player>> players; ///< Map of players, identified by UUID.
    std::unique_ptr<Map> map; ///< The game's map, including terrain and other world data.
};