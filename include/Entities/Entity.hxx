#pragma once

#include "Engine/Configuration.hxx"
#include "Entities/Functionalities/AnimationFunctionality.hxx"
#include "Entities/Functionalities/AttributeFunctionality.hxx"
#include "Entities/Functionalities/CollisionFunctionality.hxx"
#include "Entities/Functionalities/MovementFunctionality.hxx"

struct EntityData
{
    std::string name;
    sf::Vector2f currentGridPosition;
    sf::Vector2f spawnGridPosition;
};

class Entity
{
  protected:
    std::string name;
    uint64_t id; // Unique session identifier

    sf::Vector2f spawnGridPosition;

    sf::Texture &spriteSheet;
    float scale;

    // All entities have a base sprie layer by default
    std::map<std::string, std::shared_ptr<sf::Sprite>> layers;
    std::shared_ptr<sf::Sprite> baseSprite;

    std::optional<MovementFunctionality> movementFunctionality;
    std::optional<AnimationFunctionality> animationFunctionality;
    std::optional<AttributeFunctionality> attributeFunctionality;
    std::optional<CollisionFunctionality> collisionFunctionality;

    void createMovementFunctionality(const float &max_velocity, const uint8_t &movement_flags,
                                     const uint8_t &movement_direction = MovementDirection::Down,
                                     const uint8_t &movement_state = MovementState::Idle);

    void createAnimationFunctionality();

    void createAttributeFunctionality(const uint8_t &max_health, const uint8_t &max_hunger);

    void createCollisionFunctionality();

  public:
    Entity(const std::string name, const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet,
           const float &scale);

    virtual ~Entity();

    virtual void update(const float &dt, const sf::Vector2f &mouse_pos) = 0;

    virtual void render(sf::RenderTarget &target) = 0;

    void move(const float &dt, const MovementDirection &direction);

    void playAnimation(const std::string &name);

    const sf::Vector2f getPosition() const;

    std::shared_ptr<sf::Sprite> getSpriteLayer(const std::string &key);

    const sf::Vector2f getGridPosition() const;

    const sf::Vector2f getCenter() const;

    const sf::Vector2f getCenterGridPosition() const;

    AttributeFunctionality &getAttributeFunctionality();

    void setPosition(const sf::Vector2f &position);

    void setGridPosition(const sf::Vector2f &grid_position);

    void setCenterGridPosition(const sf::Vector2f &grid_position);

    /**
     * @attention The order of rendering of the layers follows the order of insertion.
     */
    void addSpriteLayer(const std::string &key);

    const std::unordered_map<std::string, HitBox> &getHitBoxes() const;

    const HitBox getHitBox(const std::string &key) const;
};