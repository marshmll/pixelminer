#pragma once

#include "Tiles/TileBase.hxx"

class Tile : public TileBase
{
  private:
  public:
    Tile(const std::string name, const std::string &id, sf::Texture &texture, const sf::IntRect &texture_rect,
         const unsigned int &grid_size, const sf::Vector2u &grid_position, const float &scale,
         const sf::Color &color = sf::Color::White);

    ~Tile();

    void update();

    void render(sf::RenderTarget &target);
};
