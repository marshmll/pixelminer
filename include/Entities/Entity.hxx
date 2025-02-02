#pragma once

#include "Engine/Configuration.hxx"
#include "Entities/Functionalities/AnimationFunctionality.hxx"
#include "Entities/Functionalities/AttributeFunctionality.hxx"
#include "Entities/Functionalities/MovementFunctionality.hxx"

struct EntityData
{
    std::string name;
    sf::Vector2f currentPosition;
    sf::Vector2f spawnPosition;
};

class Entity
{
  protected:
    std::string name;
    uint64_t id; // Unique session identifier

    sf::Vector2f spawnPosition;

    sf::Texture &spriteSheet;
    sf::Sprite sprite;

    std::optional<MovementFunctionality> movementFunctionality;
    std::optional<AnimationFunctionality> animationFunctionality;
    std::optional<AttributeFunctionality> attributeFunctionality;

    void createMovementFunctionality(const float &max_velocity, const uint8_t &movement_flags,
                                     const uint8_t &movement_direction = MovementDirection::Down,
                                     const uint8_t &movement_state = MovementState::Idle);

    void createAnimationFunctionality();

    void createAttributeFunctionality(const uint8_t &max_health, const uint8_t &max_hunger);

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

    AttributeFunctionality &getAttributeFunctionality();

    void setPosition(const sf::Vector2f &position);
};