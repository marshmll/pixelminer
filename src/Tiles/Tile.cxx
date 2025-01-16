#include "Tiles/Tile.hxx"
#include "stdafx.hxx"

Tile::Tile(const std::string name, const std::uint32_t id, sf::Texture &texture, const sf::IntRect &texture_rect,
           const unsigned int &grid_size, const sf::Vector2u &grid_position, const float &scale, const sf::Color &color)
    : TileBase(name, id, texture, texture_rect)
{
    sprite.setScale({scale, scale});
    sprite.setPosition(sf::Vector2f(grid_position.x * grid_size * scale, grid_position.y * grid_size * scale));
    sprite.setColor(color);
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