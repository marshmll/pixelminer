#pragma once

#include "Animations/Animation.hxx"

class AnimationFunctionality
{
  private:
    std::string entityName;
    std::uint64_t entityId;
    sf::Sprite &sprite;
    sf::Texture &spriteSheet;

    std::map<std::string, std::unique_ptr<Animation>> animations;

  public:
    AnimationFunctionality(const std::string &entity_name, const std::uint64_t entity_id, sf::Sprite &sprite,
                           sf::Texture &sprite_sheet);

    ~AnimationFunctionality();

    void addAnimation(const std::string name, const std::int32_t &frametime_as_ms, const sf::Vector2u frame_size,
                      const sf::Vector2u &start_frame_index, const sf::Vector2u end_frame_index,
                      const bool boomerang = false);

    void play(const std::string &name, const bool reset_others = false);

    void reset(const std::string &name);
};