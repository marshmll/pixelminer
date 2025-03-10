#include "Game/Commands/CommandImpl/IdCommandImpl.hxx"
#include "stdafx.hxx"

const std::string CommandImpl::id(GameContext &ctx, CommandContext &cmd)
{
    const auto &tokens = cmd.tokens;

    if (tokens.size() < 2 || tokens.size() > 3)
    {
        return _("Usage: ") + COMMAND_INVOKER + _("id [target]");
    }

    std::vector<Token> target_tokens(tokens.begin() + 1, tokens.end());

    // Resolve the target entity
    std::vector<Entity *> entities = resolveEntities(target_tokens, ctx, cmd);

    if (entities.empty())
    {
        return _("Error: No valid entity found for target \"") + tokens[1].literal + "\".";
    }

    if (entities.size() > 1)
    {
        return _("Error: Target \"") + tokens[1].literal +
               _("\" resolved to multiple entities. Please specify a unique target.");
    }

    // Get the ID of the resolved entity
    uint64_t entityId = entities[0]->getId();

    // Return the ID as the message
    return _("Entity ID: ") + std::to_string(entityId);
}
