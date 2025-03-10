#include "Game/Commands/CommandInterpreter.hxx"
#include "stdafx.hxx"

std::vector<Token> CommandInterpreter::tokenize(std::string_view command)
{
    std::vector<Token> tokens;
    tokens.reserve(10); // Reserve space for typical command lengths

    size_t start = 0;
    size_t end = command.find(' ');

    while (end != std::string_view::npos)
    {
        std::string_view token_str = command.substr(start, end - start);
        TokenType type = TokenType::Unknown;

        if (!token_str.empty())
        {
            if (token_str[0] == COMMAND_INVOKER_CHAR)
            {
                type = TokenType::Command;
            }
            else if (token_str[0] == '@')
            {
                type = TokenType::Target;
            }
            else if (token_str.find_first_not_of("0123456789.") == std::string_view::npos)
            {
                size_t dot_count = 0;
                for (char ch : token_str)
                {
                    if (ch == '.')
                        dot_count++;
                }

                if (dot_count == 0)
                {
                    type = TokenType::Integer;
                }
                else if (dot_count == 1)
                {
                    type = TokenType::Decimal;
                }
            }
            else
            {
                type = TokenType::String;
            }

            tokens.emplace_back(Token{type, std::string(token_str)});
        }

        start = end + 1;
        end = command.find(' ', start);
    }

    // Handle the last token
    if (start < command.size())
    {
        std::string_view token_str = command.substr(start);
        TokenType type = TokenType::Unknown;

        if (!token_str.empty())
        {
            if (token_str[0] == COMMAND_INVOKER_CHAR)
            {
                type = TokenType::Command;
            }
            else if (token_str[0] == '@')
            {
                type = TokenType::Target;
            }
            else if (token_str.find_first_not_of("0123456789.") == std::string_view::npos)
            {
                size_t dot_count = 0;
                for (char ch : token_str)
                {
                    if (ch == '.')
                        dot_count++;
                }

                if (dot_count == 0)
                {
                    type = TokenType::Integer;
                }
                else if (dot_count == 1)
                {
                    type = TokenType::Decimal;
                }
            }
            else
            {
                type = TokenType::String;
            }

            tokens.emplace_back(Token{type, std::string(token_str)});
        }
    }

    return tokens;
}

CommandInterpreter::CommandInterpreter(GameContext &ctx) : ctx(ctx)
{}

CommandInterpreter::~CommandInterpreter() = default;

const std::string CommandInterpreter::interpret(std::optional<std::string> caller_uuid, std::string_view command)
{
    CommandContext cmd;

    cmd.callerUuid = caller_uuid;
    cmd.tokens = tokenize(command);

    if (cmd.tokens.empty())
    {
        return _("Error: Empty command.");
    }

    auto it = CommandImpl::commandHandlersTable.find(cmd.tokens[0].literal);
    if (it == CommandImpl::commandHandlersTable.end())
    {
        return _("Unknown command: \"") + std::string(command) + "\"";
    }

    return it->second(ctx, cmd);
}
