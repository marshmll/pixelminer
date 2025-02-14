#include "Entities/Functionalities/MovementFunctionality.hxx"
#include "stdafx.hxx"

MovementFunctionality::MovementFunctionality(std::map<std::string, std::shared_ptr<sf::Sprite>> &layers,
                                             const float &max_velocity, const float &scale,
                                             const uint8_t &movement_flags, const uint8_t &movement_direction,
                                             const uint8_t &movement_state)
    : layers(layers), velocity(0.f, 0.f), maxVelocity(max_velocity), scale(scale), flags(movement_flags),
      state(movement_state), direction(movement_direction)
{}

MovementFunctionality::~MovementFunctionality()
{}

void MovementFunctionality::update()
{
    state = MovementState::Idle;
    velocity.x = velocity.y = 0.f;
}

void MovementFunctionality::move(const float &dt, const uint8_t direction)
{
    if (direction == MovementDirection::Up && flags & MovementAllow::AllowUp)
    {
        this->direction = direction;
        state = MovementState::Walking;
        velocity.y = std::round(-maxVelocity * scale * dt);
    }
    else if (direction == MovementDirection::Down && flags & MovementAllow::AllowDown)
    {
        this->direction = direction;
        state = MovementState::Walking;
        velocity.y = std::round(maxVelocity * scale * dt);
    }
    else if (direction == MovementDirection::Left && flags & MovementAllow::AllowLeft)
    {
        this->direction = direction;
        state = MovementState::Walking;
        velocity.x = std::round(-maxVelocity * scale * dt);
    }
    else if (direction == MovementDirection::Right && flags & MovementAllow::AllowRight)
    {
        this->direction = direction;
        state = MovementState::Walking;

        velocity.x = std::round(maxVelocity * scale * dt);
    }

    for (auto &[_, sprite] : layers)
    {
        if (sprite)
            sprite->move(velocity);
    }
}

const sf::Vector2f &MovementFunctionality::getVelocity() const
{
    return velocity;
}

const float &MovementFunctionality::getMaxVelocity() const
{
    return maxVelocity;
}

const uint8_t &MovementFunctionality::getState() const
{
    return state;
}

const uint8_t &MovementFunctionality::getDirection() const
{
    return direction;
}

const uint8_t &MovementFunctionality::getFlags() const
{
    return flags;
}

const std::string MovementFunctionality::getDirectionAsString() const
{
    std::string str;

    switch (direction)
    {
    case Up: str = "Up"; break;
    case Down: str = "Down"; break;
    case Left: str = "Left"; break;
    case Right: str = "Right"; break;
    }

    return str;
}

void MovementFunctionality::setVelocity(const sf::Vector2f &velocity)
{
    this->velocity = velocity;
}
