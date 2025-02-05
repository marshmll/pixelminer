#pragma once

#include "Entities/Entity.hxx"

class Tree : public Entity
{
  protected:
  public:
    Tree(const std::string name, const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet, const float &scale);

    virtual ~Tree();

    virtual void update(const float &dt, const sf::Vector2f &mouse_pos) = 0;

    virtual void render(sf::RenderTarget &target) = 0;

    virtual void render(sf::RenderTarget &target, const bool &show_hitboxes) = 0;
};
