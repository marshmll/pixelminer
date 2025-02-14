/**
 * @file IdCommandImpl.hxx
 * @brief Declares the id command.
 */

#pragma once

#include "Game/Commands/CommandContext.hxx"
#include "Game/Commands/CommandImpl/TargetResolver.hxx"
#include "Game/GameContext.hxx"

namespace CommandImpl
{
/**
 * @brief Executes the id command.
 *
 * Usage: id [target]
 *
 * @param ctx The game context providing entity access.
 * @param cmd The command context providing command caller information.
 * @return A string with the unique session id of a entity, or
 * an error message.
 */
const std::string id(GameContext &ctx, CommandContext &cmd);
} // namespace CommandImpl