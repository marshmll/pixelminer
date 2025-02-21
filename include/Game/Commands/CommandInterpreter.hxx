/**
 * @file CommandInterpreter.hxx
 * @brief Declares the CommandInterpreter class for interpreting and executing commands inside the GameState.
 */

#pragma once

#include "Game/Commands/Commands.hxx"
#include "Game/GameContext.hxx"

/**
 * @class CommandInterpreter
 * @brief Handles parsing and executing commands within the game context.
 */
class CommandInterpreter
{
  private:
    GameContext &ctx; ///< Reference to the game context.

    /**
     * @brief Tokenizes a command string into individual tokens.
     *
     * This function splits a given command string into separate tokens based on whitespace
     * and categorizes them accordingly (e.g., commands, targets, integers, strings).
     *
     * @param command The command string to tokenize.
     * @return A vector of parsed tokens.
     */
    std::vector<Token> tokenize(std::string_view command);

  public:
    /**
     * @brief Constructs a CommandInterpreter.
     *
     * @param ctx Reference to the game context used for command execution.
     */
    CommandInterpreter(GameContext &ctx);

    /**
     * @brief Default destructor.
     */
    ~CommandInterpreter();

    /**
     * @brief Interprets and executes a command.
     *
     * Parses the provided command, determines the appropriate handler,
     * and executes it within the game context.
     *
     * @param caller_name Name of the entity issuing the command.
     * @param caller_uuid Optional UUID of the entity issuing the command.
     * @param command The command string to interpret and execute.
     * @return The result of the command execution as a string.
     */
    const std::string
    interpret(std::optional<std::string> caller_uuid, std::string_view command);
};
