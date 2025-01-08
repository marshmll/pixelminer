#pragma once

#include "Entities/Abilities/MovementAbility.hxx"

class Entity
{
  protected:
    std::string name;
    std::uint64_t id;    // Unique identifier
    sf::Texture texture; // temp
    sf::Sprite sprite;

    std::optional<MovementAbility> movementAbility;

    void createMovementAbility(const float &max_velocity, const std::uint8_t &movement_flags);

  public:
    Entity(const std::string name, const sf::Vector2f position);
    virtual ~Entity();

    virtual void update(const float &dt);
    
    virtual void render(sf::RenderTarget &target);

    virtual void move(const float &dt, const MovementDirection &direction);
};