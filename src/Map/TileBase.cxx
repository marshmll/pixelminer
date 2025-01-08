#include "Map/TileBase.hxx"
#include "stdafx.hxx"

TileBase::TileBase(const std::string name, const std::uint32_t id, std::unique_ptr<sf::Texture> &texture,
                   const sf::IntRect &texture_rect, const float &scale)
    : name(name), id(id), texture(texture), textureRect(texture_rect), sprite(*texture, textureRect)
{
    sprite.setScale({scale, scale});
}

TileBase::~TileBase()
{
}

void TileBase::update()
{
}

void TileBase::render(sf::RenderTarget &target)
{
    target.draw(sprite);
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