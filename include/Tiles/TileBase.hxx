#pragma once

#include "Engine/Configuration.hxx"
#include "TileData.hxx"

class TileBase
{
  protected:
    std::string name;
    std::string id;
    sf::Texture &texture;
    sf::IntRect textureRect;
    sf::Sprite sprite;

  public:
    TileBase(const std::string name, const std::string &id, sf::Texture &texture, const sf::IntRect &texture_rect,
             const float &scale = 1.f);

    virtual ~TileBase();

    virtual void render(sf::RenderTarget &target);

    const std::string &getId() const;

    const std::string &getName() const;

    const sf::Vector2f getPosition() const;

    const sf::Vector2u getGridPosition() const;

    sf::Color getColor() const;

    const sf::FloatRect getGlobalBounds() const;

    const sf::Vector2f getCenter() const;

    void setPosition(const sf::Vector2f &position);

    void setGridPosition(const sf::Vector2u &position);

    void setColor(const sf::Color &color);
};