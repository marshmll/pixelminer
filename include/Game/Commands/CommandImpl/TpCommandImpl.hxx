/**
 * @file TpCommandImpl.hxx
 * @brief Declares the tp command handler.
 */

#pragma once

#include "Game/Commands/CommandContext.hxx"
#include "Game/Commands/CommandImpl/TargetResolver.hxx"
#include "Game/GameContext.hxx"

namespace CommandImpl
{
/**
 * @brief Executes the tp command.
 *
 * Usage: tp [targets...] [{x y} OR {target}]
 *
 * @param ctx The game context providing entity access.
 * @param cmd The command context providing command caller information.
 * @return A string with the amount of entities teleported, or
 * an error message.
 */
const std::string tp(GameContext &ctx, CommandContext &cmd);
}; // namespace CommandImpl