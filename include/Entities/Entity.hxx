/**
 * @file Entity.hxx
 * @brief Declares the Entity class for managing all types of entities.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "Entities/Functionalities/AnimationFunctionality.hxx"
#include "Entities/Functionalities/AttributeFunctionality.hxx"
#include "Entities/Functionalities/CollisionFunctionality.hxx"
#include "Entities/Functionalities/MovementFunctionality.hxx"
#include "Entities/Functionalities/SoundFunctionality.hxx"
#include "Entities/RenderBehavior.hxx"
#include "Tools/Logger.hxx"

class Entity;

/**
 * @struct EntityData
 * @brief Contains basic data about an entity, such as its name and position.
 */
struct EntityData
{
    std::string name;                 ///< Name of the entity.
    sf::Vector2f currentGridPosition; ///< Current grid position of the entity.
    sf::Vector2f spawnGridPosition;   ///< Spawn grid position of the entity.
};

/**
 * @enum EntityType
 * @brief Specify types of entity.
 */
enum EntityType : uint8_t
{
    InanimatedEntity, ///< An inanimated entity, generally a object.
    MobEntity,        ///< A monster, animal or any similar entity.
    NpcEntity,        ///< Interactible entity.
    PlayerEntity      ///< Playable entity.
};

/**
 * @class Entity
 * @brief Base class for all entities in the game. Handles common functionality like movement, animation, and collision.
 */
class Entity
{
  protected:
    Logger logger; ///< Logger instance for logging messages.

    std::string name; ///< Name of the entity.
    uint8_t type;     ///< Type of entity (e.g., inanimated, mob, etc.).
    uint64_t id;      ///< Unique session identifier for the entity.

    sf::Vector2f spawnGridPosition; ///< Spawn position of the entity in grid coordinates.

    sf::Texture &spriteSheet; ///< Reference to the sprite sheet texture used by the entity.
    float scale;              ///< Scaling factor for the entity's sprites.

    std::unordered_map<std::string, sf::SoundBuffer>
        &soundBuffers; ///< A reference to the resource pack sound buffer map

    uint8_t renderBehavior; ///< Rendering behavior relative to other entities.

    std::map<std::string, std::shared_ptr<sf::Sprite>> layers; ///< Map of sprite layers for the entity.
    std::shared_ptr<sf::Sprite> baseSprite;                    ///< Base sprite layer for the entity.

    std::optional<MovementFunctionality> movementFunctionality;   ///< Optional movement functionality.
    std::optional<AnimationFunctionality> animationFunctionality; ///< Optional animation functionality.
    std::optional<AttributeFunctionality> attributeFunctionality; ///< Optional attribute functionality.
    std::optional<CollisionFunctionality> collisionFunctionality; ///< Optional collision functionality.
    std::optional<SoundFunctionality> soundFunctionality;         ///< Optional sound functionality.

    std::optional<sf::FloatRect> collisionRect; ///< Optional collision rectangle for the entity.

    /**
     * @brief Creates the movement functionality for the entity.
     * @param max_velocity Maximum velocity of the entity.
     * @param movement_flags Bitmask flags for allowed movements.
     * @param movement_direction Initial movement direction (default: Down).
     * @param movement_state Initial movement state (default: Idle).
     */
    void createMovementFunctionality(const float &max_velocity, const uint8_t &movement_flags,
                                     const uint8_t &movement_direction = MovementDirection::Down,
                                     const uint8_t &movement_state = MovementState::Idle);

    /**
     * @brief Creates the animation functionality for the entity.
     */
    void createAnimationFunctionality();

    /**
     * @brief Creates the attribute functionality for the entity.
     * @param max_health Maximum health of the entity.
     * @param max_hunger Maximum hunger of the entity.
     */
    void createAttributeFunctionality(const uint8_t &max_health, const uint8_t &max_hunger);

    /**
     * @brief Creates the collision functionality for the entity.
     */
    void createCollisionFunctionality();

    /**
     * @brief Creates the sound functionality for the entity.
     */
    void createSoundFunctionality();

  public:
    /**
     * @brief Constructs an Entity object.
     * @param name Name of the entity.
     * @param type Type of the entity.
     * @param spawn_grid_position Spawn position of the entity in grid coordinates.
     * @param sprite_sheet Reference to the sprite sheet texture.
     * @param scale Scaling factor for the entity's sprites.
     * @param sound_buffers A reference to the resource pack sound buffers unordered_map.
     * @param render_behavior The render behavior for the entity's sprites.
     */
    Entity(const std::string &name, const std::uint8_t &type, const sf::Vector2f &spawn_grid_position,
           sf::Texture &sprite_sheet, const float &scale,
           std::unordered_map<std::string, sf::SoundBuffer> &sound_buffers,
           const uint8_t &render_behavior = RenderBehavior::Flat);

    /**
     * @brief Destructor for the Entity class.
     */
    virtual ~Entity();

    /**
     * @brief Updates the entity's state.
     * @param dt Delta time for frame-independent updates.
     * @param mouse_pos Current position of the mouse.
     */
    virtual void update(const float &dt, const sf::Vector2f &mouse_pos) = 0;

    /**
     * @brief Renders the entity on the target.
     * @param target Render target to draw the entity on.
     */
    virtual void render(sf::RenderTarget &target) = 0;

    /**
     * @brief Renders the entity on the target, optionally showing hitboxes.
     * @param target Render target to draw the entity on.
     * @param show_hitboxes Whether to render hitboxes.
     */
    virtual void render(sf::RenderTarget &target, const bool &show_hitboxes) = 0;

    /**
     * @brief Moves the entity in the specified direction.
     * @param dt Delta time for frame-independent movement.
     * @param direction Direction to move in.
     */
    void move(const float &dt, const MovementDirection &direction);

    /**
     * @brief Moves the entity by a specified offset.
     * @param offset Offset to move the entity by.
     */
    void move(const sf::Vector2f &offset);

    /**
     * @brief Plays an animation by name.
     * @param name Name of the animation to play.
     */
    void playAnimation(const std::string &name);

    /**
     * @brief Gets the name of the entity.
     * @return Name of the entity.
     */
    const std::string &getName() const;

    /**
     * @brief Gets the type of the entity.
     * @return Type of the entity.
     */
    const uint8_t &getType() const;

    /**
     * @brief Gets the unique ID of the entity.
     * @return Unique ID of the entity.
     */
    const uint64_t &getId() const;

    /**
     * @brief Gets the current position of the base sprite of the entity.
     * @return Current position of the base sprite of the entity.
     */
    const sf::Vector2f getPosition() const;

    /**
     * @brief Gets the current size of the the base sprite of the entity.
     * @return Current size of the base sprite of the entity.
     */
    const sf::Vector2f getSize() const;

    /**
     * @brief Gets the current velocity of the entity.
     * @return Current velocity of the entity.
     */
    const sf::Vector2f getVelocity();

    /**
     * @brief Checks if the entity is movable.
     * @return True if the entity has movement functionality and it allows any movement, false otherwise.
     */
    const bool canMove();

    /**
     * @brief Checks if the entity is currently moving.
     * @return True if the entity is moving, false otherwise.
     */
    const bool isMoving();

    /**
     * @brief Gets a sprite layer by key.
     * @param key Key of the sprite layer.
     * @return Shared pointer to the sprite layer, or nullptr if not found.
     */
    std::shared_ptr<sf::Sprite> getSpriteLayer(const std::string &key);

    /**
     * @brief Gets the current grid position of the entity.
     * @return Current grid position of the entity.
     */
    const sf::Vector2f getGridPosition() const;

    /**
     * @brief Gets the position of the first hitbox.
     * @return Position of the first hitbox.
     */
    const sf::Vector2f getFirstHitBoxPosition();

    /**
     * @brief Gets the grid position of the first hitbox.
     * @return Grid position of the first hitbox.
     */
    const sf::Vector2f getFirstHitBoxGridPosition();

    /**
     * @brief Gets the size of the first hitbox.
     * @return Size of the first hitbox.
     */
    const sf::Vector2f getFirstHitBoxSize();

    /**
     * @brief Gets the center position of the entity.
     * @return Center position of the entity.
     */
    const sf::Vector2f getCenter() const;

    /**
     * @brief Gets the center grid position of the entity.
     * @return Center grid position of the entity.
     */
    const sf::Vector2f getCenterGridPosition() const;

    /**
     * @brief Gets the bottom grid position of the entity.
     * @return Bottom grid position of the entity.
     */
    const sf::Vector2f getBottomGridPosition() const;

    /**
     * @brief Gets the attribute functionality of the entity.
     * @return Reference to the attribute functionality.
     */
    AttributeFunctionality &getAttributeFunctionality();

    /**
     * @brief Gets the hitboxes of the entity.
     * @return Map of hitboxes.
     */
    std::map<std::string, HitBox> &getHitBoxes();

    /**
     * @brief Gets a specific hitbox by key.
     * @param key Key of the hitbox.
     * @return Hitbox corresponding to the key.
     */
    const HitBox getHitBox(const std::string &key) const;

    /**
     * @brief Checks if the entity is collideable.
     * @return True if the entity has collision functionality, false otherwise.
     */
    const bool isCollideable() const;

    /**
     * @brief Gets the collision rectangle of the entity.
     * @return Optional collision rectangle.
     */
    const std::optional<sf::FloatRect> &getCollisionRect() const;

    /**
     * @brief Gets the render behavior of the entity.
     * @return Render behavior of the entity.
     */
    const uint8_t &getRenderBehavior() const;

    /**
     * @brief Sets the position of the entity.
     * @param position New position of the entity.
     */
    void setPosition(const sf::Vector2f &position);

    /**
     * @brief Sets the grid position of the entity.
     * @param grid_position New grid position of the entity.
     */
    void setGridPosition(const sf::Vector2f &grid_position);

    /**
     * @brief Sets the center grid position of the entity.
     * @param grid_position New center grid position of the entity.
     */
    void setCenterGridPosition(const sf::Vector2f &grid_position);

    /**
     * @brief Sets the position of the first hitbox.
     * @param position New position of the first hitbox.
     */
    void setHitBoxPosition(const sf::Vector2f &position);

    /**
     * @brief Sets the collision rectangle of the entity.
     * @param rect New collision rectangle.
     */
    void setCollisionRect(const std::optional<sf::FloatRect> &rect);

    /**
     * @brief Adds a sprite layer to the entity.
     * @param key Key for the new sprite layer.
     * @attention The order of rendering of the layers follows the order of insertion.
     */
    void addSpriteLayer(const std::string &key);
};
