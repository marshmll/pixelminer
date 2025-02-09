/**
 * @file MovementFunctionality.hxx
 * @brief Declares the MovementFunctionality class for managing entity movement.
 */

#pragma once

/**
 * @enum MovementAllow
 * @brief Bitmask flags representing allowed movement actions.
 */
enum MovementAllow : uint8_t
{
    AllowNone = 0x0,    ///< Allow no movement.
    AllowUp = 0x1,      ///< Allow moving upwards.
    AllowDown = 0x2,    ///< Allow moving downwards.
    AllowLeft = 0x4,    ///< Allow moving leftwards.
    AllowRight = 0x8,   ///< Allow moving rightwards.
    AllowSprint = 0x10, ///< Allow sprinting (depends on movement allow).
    AllowJump = 0x20,   ///< Allow jumping.
    AllowCrouch = 0x40, ///< Allow crouching.
    AllowAll = 0xFF,    ///< Allow all movements.
};

/**
 * @enum MovementState
 * @brief Represents the current state of movement.
 */
enum MovementState : uint8_t
{
    Idle = 0,  ///< No movement.
    Walking,   ///< Walking movement.
    Sprinting, ///< Sprinting movement.
    Jumping,   ///< Jumping movement.
    Crouching, ///< Crouching movement.
};

/**
 * @enum MovementDirection
 * @brief Represents the direction of movement.
 */
enum MovementDirection : uint8_t
{
    Up = 0, ///< Upwards direction.
    Down,   ///< Downwards direction.
    Left,   ///< Leftwards direction.
    Right   ///< Rightwards direction.
};

/**
 * @class MovementFunctionality
 * @brief Handles movement functionality for an entity, including velocity, state, and direction.
 */
class MovementFunctionality
{
  private:
    std::map<std::string, std::shared_ptr<sf::Sprite>> &layers; ///< Reference to sprite layers.
    sf::Vector2f velocity;                                      ///< Current velocity of the entity.
    float maxVelocity;                                          ///< Maximum velocity the entity can achieve.
    float scale;                                                ///< Scaling factor for movement.
    uint8_t flags;                                              ///< Bitmask flags for allowed movements.
    uint8_t state;                                              ///< Current movement state.
    uint8_t direction;                                          ///< Current movement direction.

  public:
    /**
     * @brief Constructs a MovementFunctionality object.
     * @param layers Reference to a map of sprite layers.
     * @param max_velocity Maximum velocity of the entity.
     * @param scale Scaling factor for movement.
     * @param movement_flags Bitmask flags for allowed movements.
     * @param movement_direction Initial movement direction (default: Down).
     * @param movement_state Initial movement state (default: Idle).
     */
    MovementFunctionality(std::map<std::string, std::shared_ptr<sf::Sprite>> &layers, const float &max_velocity,
                          const float &scale, const uint8_t &movement_flags,
                          const uint8_t &movement_direction = MovementDirection::Down,
                          const uint8_t &movement_state = MovementState::Idle);

    /**
     * @brief Destructor for MovementFunctionality.
     */
    ~MovementFunctionality();

    /**
     * @brief Updates the movement state and resets velocity.
     */
    void update();

    /**
     * @brief Moves the entity in the specified direction.
     * @param dt Delta time for frame-independent movement.
     * @param direction Direction to move in.
     */
    void move(const float &dt, const uint8_t direction);

    /**
     * @brief Gets the current velocity of the entity.
     * @return Current velocity as a 2D vector.
     */
    const sf::Vector2f &getVelocity() const;

    /**
     * @brief Gets the maximum velocity of the entity.
     * @return Maximum velocity as a float.
     */
    const float &getMaxVelocity() const;

    /**
     * @brief Gets the current movement state of the entity.
     * @return Current movement state as a uint8_t.
     */
    const uint8_t &getState() const;

    /**
     * @brief Gets the current movement direction of the entity.
     * @return Current movement direction as a uint8_t.
     */
    const uint8_t &getDirection() const;

    /**
     * @brief Gets the movement flags of the entity.
     * @return Movement flags as a uint8_t.
     */
    const uint8_t &getFlags() const;

    /**
     * @brief Converts the current movement direction to a string.
     * @return Direction as a string (e.g., "Up", "Down").
     */
    const std::string getDirectionAsString() const;

    /**
     * @brief Sets the velocity of the entity.
     * @param velocity New velocity as a 2D vector.
     */
    void setVelocity(const sf::Vector2f &velocity);
};