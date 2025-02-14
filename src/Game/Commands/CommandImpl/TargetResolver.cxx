#include "Game/Commands/CommandImpl/TargetResolver.hxx"
#include "stdafx.hxx"

std::vector<Entity *>
CommandImpl::resolveEntities(const std::vector<Token> &tokens, GameContext &ctx, CommandContext &cmd)
{
    std::vector<Entity *> entities;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i].type == TokenType::Target && tokens[i].literal == "@p" && i + 1 < tokens.size() &&
            tokens[i + 1].type == TokenType::String)
        {
            // Resolve @p with player name
            auto resolved = CommandImpl::resolveTargetWithName(tokens[i], tokens[i + 1], ctx, cmd);
            entities.insert(entities.end(), resolved.begin(), resolved.end());
            ++i; // Skip the name token
        }
        else
        {
            // Resolve standalone target
            auto resolved = CommandImpl::resolveTarget(tokens[i], ctx, cmd);
            entities.insert(entities.end(), resolved.begin(), resolved.end());
        }
    }

    return entities;
}

std::vector<Entity *> CommandImpl::resolveTarget(const Token &token, GameContext &ctx, CommandContext &cmd)
{
    std::vector<Entity *> entities;

    if (token.type == TokenType::Target && token.literal == "@p")
    { // Caller's player or player by name
        // Default to the caller's player
        if (cmd.callerUuid)
        {
            auto it = ctx.players.find(*cmd.callerUuid);
            if (it != ctx.players.end())
            {
                entities.push_back(it->second.get());
            }
        }
    }
    else if (token.type == TokenType::Target && token.literal == "@e")
    { // All global entities
        for (auto &e : ctx.globalEntities)
        {
            entities.push_back(e.get());
        }
    }
    else if (token.type == TokenType::Integer)
    { // ID-based targeting
        try
        {
            uint64_t id = std::stoll(token.literal); // Parse the 64-bit ID

            // Check global entities
            for (auto &e : ctx.globalEntities)
            {
                if (e->getId() == id)
                {
                    entities.push_back(e.get());
                    break;
                }
            }
        }
        catch (const std::invalid_argument &)
        {
            // Ignore invalid IDs (non-numeric tokens)
        }
    }
    return entities;
}

std::vector<Entity *> CommandImpl::resolveTargetWithName(const Token &targetToken, const Token &nameToken,
                                                         GameContext &ctx, CommandContext &cmd)
{
    std::vector<Entity *> entities;

    if (targetToken.type == TokenType::Target && targetToken.literal == "@p" && nameToken.type == TokenType::String)
    {
        // Resolve player by name
        std::string playerName = nameToken.literal;
        for (const auto &[uuid, player] : ctx.players)
        {
            if (player->getName() == playerName)
            {
                entities.push_back(player.get());
                break;
            }
        }
    }
    else
    {
        // Fallback to regular target resolution
        entities = resolveTarget(targetToken, ctx, cmd);
    }

    return entities;
}