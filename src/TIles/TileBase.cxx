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

const std::uint32_t &TileBase::getId() const
{
    return id;
}

const sf::Vector2f TileBase::getPosition() const
{
    return sprite.getPosition();
}

const sf::FloatRect TileBase::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}

const sf::Vector2f TileBase::getCenter() const
{
    return sprite.getGlobalBounds().getCenter();
}