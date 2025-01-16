#pragma once

#include "Entities/Entity.hxx"

struct PlayerData : EntityData
{
    std::string uuid;
};

class Player : public Entity
{
  private:
    void initPlayerAnimations();

  public:
    Player(const sf::Vector2f spawn_position, sf::Texture &sprite_sheet, const float &scale);
    ~Player();

    void update(const float &dt, const bool &update_movement = true);

    void render(sf::RenderTarget &target);
};