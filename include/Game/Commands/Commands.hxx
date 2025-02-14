/**
 * @file Commands.hxx
 * @brief Aggregates all command handler implementations in the CommandImpl namespace.
 */

#pragma once

#include "Game/Commands/CommandImpl/IdCommandImpl.hxx"
#include "Game/Commands/CommandImpl/TpCommandImpl.hxx"

/**
 * @namespace CommandImpl
 * @brief Contains implementations for in-game command handlers.
 */
namespace CommandImpl
{

    /**
     * @brief A look-up table associating a command call string with its respective handler.
     */
    static const std::unordered_map<std::string, std::function<const std::string(GameContext &, CommandContext &)>>
        commandHandlersTable = {{COMMAND_INVOKER + "id", id}, {COMMAND_INVOKER + "tp", tp}};
}; // namespace CommandImpl