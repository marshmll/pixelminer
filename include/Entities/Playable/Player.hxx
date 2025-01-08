#pragma once

#include "Entities/Entity.hxx"

class Player : public Entity
{
  private:
  public:
    Player(const sf::Vector2f spawn_position, sf::Texture &sprite_sheet, const float &scale);
    ~Player();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};