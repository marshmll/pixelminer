#pragma once

#include "Entities/Inanimated/Trees/Tree.hxx"

class PineTree : public Tree
{
  private:
    void initAnimations();

  public:
    PineTree(const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet, const float &scale);

    ~PineTree();

    void update(const float &dt, const sf::Vector2f &mouse_pos);

    void render(sf::RenderTarget &target);
};
