#pragma once

#include "Engine/Configuration.hxx"
#include "Entities/Functionalities/AnimationFunctionality.hxx"
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
    std::optional<AnimationFunctionality> animationFunctionality;

    void createMovementFunctionality(const float &max_velocity, const std::uint8_t &movement_flags);

    void createAnimationFunctionality();

  public:
    Entity(const std::string name, const sf::Vector2f spawn_position, sf::Texture &sprite_sheet, const float &scale);
    virtual ~Entity();

    virtual void update(const float &dt);

    virtual void render(sf::RenderTarget &target);

    void move(const float &dt, const MovementDirection &direction);

    void playAnimation(const std::string &name);

    const sf::Vector2f getPosition() const;

    const sf::Vector2i getGridPosition() const;

    const sf::Vector2f getCenter() const;

    const sf::Vector2i getCenterGridPosition() const;

    void setPosition(const sf::Vector2f &position);
};