#pragma once

enum Movement : std::uint8_t
{
    AllowUp = 0b00000001,    // Allow moving upwards
    AllowDown = 0b00000010,  // Allow moving downwards
    AllowLeft = 0b00000100,  // Allow moving leftwards
    AllowRight = 0b00001000, // Allow moving rightwards
    AllowAll = 0b00001111,   // Allow moving all directions
};

enum MovementState
{
    Idle = 0,
    Moving,
    Sprinting,
    Jumping,
    Crouching,
};

enum MovementDirection
{
    Up = 0,
    Down,
    Left,
    Right
};

class MovementFunctionality
{
  private:
    sf::Sprite &sprite;
    float maxVelocity;
    std::uint8_t movementFlags;

    MovementState state;
    MovementDirection direction;

  public:
    MovementFunctionality(sf::Sprite &sprite, const float max_velocity, const std::uint8_t movement_flags);

    ~MovementFunctionality();

    void move(const float &dt, const MovementDirection direction);

    const MovementState &getState() const;

    const MovementDirection &getDirection() const;
};