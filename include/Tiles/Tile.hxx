#pragma once

#include "Tiles/TileBase.hxx"

class Tile : public TileBase
{
  private:
    unsigned int gridSize;
    sf::Vector2u gridPosition;

  public:
    Tile(const std::string name, const std::uint32_t id, sf::Texture &texture, const sf::IntRect &texture_rect,
         const unsigned int &grid_size, const sf::Vector2u &grid_position, const float &scale);

    ~Tile();

    void update();

    void render(sf::RenderTarget &target);
};
