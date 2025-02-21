/**
 * @file CommandContext.hxx
 * @brief Declares enums and structs and defines a invoker
 * used to parse a command context.
 */

#pragma once

/**
 * @brief The char value of the symbol that invokes a command call.
 */
#define COMMAND_INVOKER_CHAR '\\'

/**
 * @brief The command invoker char casted to a string.
 */
#define COMMAND_INVOKER static_cast<std::string>("\\")

/**
 * @enum TokenType
 * @brief Defines types of tokens used to parse and evaluate
 * commands
 */
enum class TokenType : short
{
    Unknown = -1, ///< Unknown token
    Integer,      ///< Number with no decimal parts
    Decimal,      ///< Number with decimal parts
    String,       ///< Any sequence of chars
    Command,      ///< A sequence of chars preceded with the COMMAND_INVOKER
    Target,       ///< A sequence of chars preceded with the target ('@') char.
};

/**
 * @struct Token
 * @brief Represents a part of a bigger command token chain.
 *
 * A token represents the smaller unit used by the evaluator.
 */
struct Token
{
    TokenType type;      ///< The TokenType of the token.
    std::string literal; ///< The literal value of the token, as a string.
};

/**
 * @struct CommandContext
 * @brief Represents the context of a command call.
 */
struct CommandContext
{
    std::optional<std::string> callerUuid; ///< A optional uuid associated with the caller.
    std::vector<Token> tokens;             ///< A tokenized command.
};
