#include "Tiles/TileBase.hxx"
#include "stdafx.hxx"

TileBase::TileBase(const std::string name, const std::uint32_t id, sf::Texture &texture,
                   const sf::IntRect &texture_rect, const float &scale)
    : name(name), id(id), texture(texture), textureRect(texture_rect), sprite(texture, textureRect)
{
    sprite.setScale({scale, scale});
}

TileBase::~TileBase()
{
}

void TileBase::render(sf::RenderTarget &target)
{
    target.draw(sprite);
}

const std::uint32_t &TileBase::getId() const
{
    return id;
}

const sf::Vector2f TileBase::getPosition() const
{
    return sprite.getPosition();
}

const sf::Color TileBase::getColor() const
{
    return sprite.getColor();
}

const sf::FloatRect TileBase::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}

const sf::Vector2f TileBase::getCenter() const
{
    return sprite.getGlobalBounds().getCenter();
}

void TileBase::setPosition(const sf::Vector2f &position)
{
    sprite.setPosition(position);
}

void TileBase::setGridPosition(const sf::Vector2u &position)
{
    sprite.setPosition(
        sf::Vector2f(position.x * GRID_SIZE * sprite.getScale().x, position.y * GRID_SIZE * sprite.getScale().y));
}

void TileBase::setColor(const sf::Color &color)
{
    sprite.setColor(color);
}