#include "Tiles/Tile.hxx"
#include "stdafx.hxx"

Tile::Tile(const std::string name, const std::string &tag, const uint64_t &id, sf::Texture &texture,
           const sf::IntRect &texture_rect, const sf::Vector2i &grid_position, const unsigned int &scale,
           const sf::Color &color)
    : TileBase(name, tag, id, texture, texture_rect, grid_position, scale)
{
    setColor(color);
}

Tile::Tile(const std::string name, const std::string &tag, const uint64_t &id, sf::Texture &texture,
           const sf::IntRect &texture_rect, const unsigned int &scale, const sf::Color &color)
    : TileBase(name, tag, id, texture, texture_rect, sf::Vector2i(), scale)
{
    setColor(color);
}

Tile::~Tile() = default;

void Tile::update()
{}
