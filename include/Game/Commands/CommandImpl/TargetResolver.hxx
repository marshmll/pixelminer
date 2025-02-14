/**
 * @file TargetResolver.hxx
 * @brief Declare functions to resolve entities based on target aliases.
 */

#pragma once

#include "Entities/Entity.hxx"
#include "Game/Commands/CommandContext.hxx"
#include "Game/GameContext.hxx"

namespace CommandImpl
{
    /**
     * @brief Resolves entities from a list of tokens.
     *
     * This function processes a vector of tokens and determines the corresponding entities
     * within the game context.
     *
     * @param tokens The vector of tokens to resolve.
     * @param ctx The game context providing entity access.
     * @param cmd The command context providing command caller information.
     * @return A vector of pointers to the resolved entities.
     */
    std::vector<Entity *> resolveEntities(const std::vector<Token> &tokens, GameContext &ctx, CommandContext &cmd);

    /**
     * @brief Resolves a target entity based on a single token.
     *
     * Supports resolving special target selectors such as "@p" (closest player or command caller)
     * and "@e" (all entities). Also allows entity resolution based on numeric IDs.
     *
     * @param token The token representing the target.
     * @param ctx The game context providing entity access.
     * @param cmd The command context providing command caller information.
     * @return A vector of pointers to the resolved entities.
     */
    std::vector<Entity *> resolveTarget(const Token &token, GameContext &ctx, CommandContext &cmd);

    /**
     * @brief Resolves a target entity based on a selector and a name.
     *
     * Used primarily for resolving players by name when "@p" is specified alongside a string token.
     * Falls back to regular target resolution if the target type does not match expected criteria.
     *
     * @param targetToken The token representing the target selector (e.g., "@p").
     * @param nameToken The token representing the entity name.
     * @param ctx The game context providing entity access.
     * @param cmd The command context providing command caller information.
     * @return A vector of pointers to the resolved entities.
     */
    std::vector<Entity *>
    resolveTargetWithName(const Token &targetToken, const Token &nameToken, GameContext &ctx, CommandContext &cmd);

} // namespace CommandImpl
