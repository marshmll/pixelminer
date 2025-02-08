#pragma once

enum Movement : uint8_t
{
    AllowUp = 0b00000001,     // Allow moving upwards
    AllowDown = 0b00000010,   // Allow moving downwards
    AllowLeft = 0b00000100,   // Allow moving leftwards
    AllowRight = 0b00001000,  // Allow moving rightwards
    AllowSprint = 0b00010000, // Allow sprinting (depends on movement allow)
    AllowJump = 0b00100000,   // Allow jumping
    AllowCrouch = 0b01000000, // Allow crouch
    AllowAll = 0b11111111,    // Allow all movements
};

enum MovementState : uint8_t
{
    Idle = 0,
    Walking,
    Sprinting,
    Jumping,
    Crouching,
};

enum MovementDirection : uint8_t
{
    Up = 0,
    Down,
    Left,
    Right
};

class MovementFunctionality
{
  private:
    std::map<std::string, std::shared_ptr<sf::Sprite>> &layers;
    sf::Vector2f velocity;
    float maxVelocity;
    float scale;
    uint8_t flags;
    uint8_t state;
    uint8_t direction;

  public:
    MovementFunctionality(std::map<std::string, std::shared_ptr<sf::Sprite>> &layers, const float &max_velocity,
                          const float &scale, const uint8_t &movement_flags,
                          const uint8_t &movement_direction = MovementDirection::Down,
                          const uint8_t &movement_state = MovementState::Idle);

    ~MovementFunctionality();

    void update();

    void move(const float &dt, const uint8_t direction);

    const sf::Vector2f &getVelocity() const;

    const float &getMaxVelocity() const;

    const uint8_t &getState() const;

    const uint8_t &getDirection() const;

    const uint8_t &getFlags() const;

    const std::string getDirectionAsString() const;

    void setVelocity(const sf::Vector2f &velocity);
};