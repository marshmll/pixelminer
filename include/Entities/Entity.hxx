#pragma once

#include "Entities/Functionalities/MovementFunctionality.hxx"

class Entity
{
  protected:
    std::string name;
    std::uint64_t id; // Unique identifier

    sf::Vector2f spawnPosition;

    sf::Texture &spriteSheet;
    sf::Sprite sprite;

    std::optional<MovementFunctionality> movementFunctionality;

    void createMovementFunctionality(const float &max_velocity, const std::uint8_t &movement_flags);

  public:
    Entity(const std::string name, const sf::Vector2f spawn_position, sf::Texture &sprite_sheet, const float &scale);
    virtual ~Entity();

    virtual void update(const float &dt);

    virtual void render(sf::RenderTarget &target);

    void move(const float &dt, const MovementDirection &direction);

    const sf::Vector2f getPosition() const;

    void setPosition(const sf::Vector2f &position);
};