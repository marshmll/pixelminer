#include "Game/Commands/CommandImpl/TpCommandImpl.hxx"
#include "stdafx.hxx"

const std::string CommandImpl::tp(GameContext &ctx, CommandContext &cmd)
{
    const auto &tokens = cmd.tokens;

    if (tokens.size() <= 2)
    {
        return _("Usage: ") + COMMAND_INVOKER + _("tp [(targets...)] [(x y) OR (target)]");
    }

    // Split tokens into source and destination parts
    std::vector<Token> sourceTokens;
    std::vector<Token> destTokens;

    // Check if the last two tokens are coordinates
    bool destIsCoords = false;
    if (tokens.size() >= 2)
    {
        const Token &secondLast = tokens[tokens.size() - 2];
        const Token &last = tokens.back();

        if ((secondLast.type == TokenType::Integer || secondLast.type == TokenType::Decimal) &&
            (last.type == TokenType::Integer || last.type == TokenType::Decimal))
        {
            destIsCoords = true;
            sourceTokens = std::vector<Token>(tokens.begin(), tokens.end() - 2);
            destTokens = std::vector<Token>(tokens.end() - 2, tokens.end());
        }
    }

    if (!destIsCoords)
    {
        // Destination is a target (could be a single token or a target followed by a name)
        if (tokens.size() >= 2 && tokens[tokens.size() - 2].type == TokenType::Target &&
            tokens.back().type == TokenType::String)
        {
            // Handle cases like "@p username"
            sourceTokens = std::vector<Token>(tokens.begin(), tokens.end() - 2);
            destTokens = std::vector<Token>(tokens.end() - 2, tokens.end());
        }
        else
        {
            // Handle single-token destination
            sourceTokens = std::vector<Token>(tokens.begin(), tokens.end() - 1);
            destTokens = std::vector<Token>(tokens.end() - 1, tokens.end());
        }
    }

    if (sourceTokens.empty())
    {
        return _("Error: No targets to teleport specified.");
    }

    // Resolve source entities
    std::vector<Entity *> sourceEntities = resolveEntities(sourceTokens, ctx, cmd);
    if (sourceEntities.empty())
    {
        return _("Error: No targets to teleport found.");
    }

    // Resolve destination
    sf::Vector2f destPos;
    if (destIsCoords)
    {
        try
        {
            destPos.x = std::stof(destTokens[0].literal);
            destPos.y = std::stof(destTokens[1].literal);

            if (destPos.x < 0.f || destPos.y < 0.f || destPos.x >= MAX_WORLD_GRID_SIZE.x ||
                destPos.y >= MAX_WORLD_GRID_SIZE.y)
                return _("Error: World coordinate out of bounds.");
        }
        catch (const std::invalid_argument &)
        {
            return _("Error: Invalid coordinate format.");
        }
    }
    else
    {
        // Destination is a target (could be a single token or a target followed by a name)
        std::vector<Entity *> destEntities = resolveEntities(destTokens, ctx, cmd);
        if (destEntities.size() == 0)
        {
            return _("Error: No destination target found.");
        }
        else if (destEntities.size() != 1)
        {
            return _("Error: Destination must resolve to exactly one target.");
        }
        destPos = destEntities[0]->getGridPosition();
    }

    // Perform teleportation
    for (auto entity : sourceEntities)
    {
        entity->setGridPosition(destPos);
    }

    // Build response message
    std::string message = _("Teleported ") + std::to_string(sourceEntities.size()) + _(" entit") +
                          (sourceEntities.size() == 1 ? _("y") : _("ies")) + _(" to ");
    if (destIsCoords)
    {
        message += "x: " + std::to_string(destPos.x) + ", y: " + std::to_string(destPos.y);
    }
    else
    {
        message += _("position of ") + destTokens[0].literal;
        if (destTokens.size() > 1)
        {
            message += " " + destTokens[1].literal;
        }
    }
    return message + ".";
}