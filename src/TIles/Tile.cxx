#include "Tiles/Tile.hxx"
#include "stdafx.hxx"

Tile::Tile(const std::string name, const std::uint32_t id, sf::Texture &texture, const sf::IntRect &texture_rect,
           const unsigned int &grid_size, const sf::Vector2u &grid_position, const float &scale)
    : TileBase(name, id, texture, texture_rect), gridSize(grid_size), gridPosition(grid_position)
{
    sprite.setPosition(sf::Vector2f(gridPosition));
}

Tile::~Tile()
{
}

void Tile::update()
{
}

void Tile::render(sf::RenderTarget &target)
{
    target.draw(sprite);
}