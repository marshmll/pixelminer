#pragma once

class TileBase
{
  protected:
    std::string name;
    std::uint8_t id;
    sf::Texture &texture;
    sf::IntRect textureRect;
    sf::Sprite sprite;

  public:
    TileBase(const std::string name, const std::uint32_t id, sf::Texture &texture, const sf::IntRect &texture_rect,
             const float &scale = 1.f);

    virtual ~TileBase();

    virtual void render(sf::RenderTarget &target) = 0;

    const sf::Vector2f getPosition() const;

    const sf::FloatRect getGlobalBounds() const;

    const sf::Vector2f getCenter() const;
};