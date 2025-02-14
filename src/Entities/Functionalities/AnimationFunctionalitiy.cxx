#include "Entities/Functionalities/AnimationFunctionality.hxx"
#include "stdafx.hxx"

AnimationFunctionality::AnimationFunctionality(const std::string &entity_name, const std::uint64_t entity_id,
                                               std::map<std::string, std::shared_ptr<sf::Sprite>> &layers,
                                               sf::Texture &sprite_sheet)

    : logger("AnimationFunctionality"), entityName(entity_name), entityId(entity_id), layers(layers),
      spriteSheet(sprite_sheet)
{}

AnimationFunctionality::~AnimationFunctionality()
{}

void AnimationFunctionality::addAnimation(const std::string &layer, const std::string name,
                                          const std::int32_t &frametime_as_ms, const sf::Vector2u &frame_size,
                                          const sf::Vector2u &start_frame_index, const sf::Vector2u &end_frame_index,
                                          const bool &boomerang)
{
    if (layers.count(layer) == 0)
    {
        logger.logError("Invalid layer \"" + layer + "\" for entity " + entityName +
                            " (ID: " + std::to_string(entityId) + ")",
                        false);

        return;
    }

    animations[name] = std::make_unique<Animation>(*layers.at(layer), spriteSheet, frametime_as_ms, frame_size,
                                                   start_frame_index, end_frame_index, boomerang);
}

void AnimationFunctionality::play(const std::string &name, const bool reset_others)
{
    try
    {
        if (reset_others)
        {
            for (auto &[animaiton_name, animation] : animations)
            {
                if (animaiton_name != name)
                    animation->reset();
            }
        }

        animations.at(name)->play();
    }
    catch (std::exception e)
    {
        logger.logError("Invalid animation \"" + name + "\" for entity " + entityName +
                            " (ID: " + std::to_string(entityId) + "): " + e.what(),
                        false);
    }
}

void AnimationFunctionality::reset(const std::string &name)
{
    try
    {
        animations.at(name)->reset();
    }
    catch (std::exception e)
    {
        logger.logError("Invalid animation \"" + name + "\" for entity " + entityName +
                            " (ID: " + std::to_string(entityId) + ")",
                        false);
    }
}