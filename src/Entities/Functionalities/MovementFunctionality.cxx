#include "Entities/Functionalities/MovementFunctionality.hxx"
#include "stdafx.hxx"

MovementFunctionality::MovementFunctionality(sf::Sprite &sprite, const float max_velocity,
                                             const std::uint8_t movement_flags)
    : sprite(sprite), maxVelocity(max_velocity), movementFlags(movement_flags), state(MovementState::Idle),
      direction(MovementDirection::Down)
{
}

MovementFunctionality::~MovementFunctionality()
{
}

void MovementFunctionality::update()
{
    state = MovementState::Idle;
}

void MovementFunctionality::move(const float &dt, const MovementDirection direction)
{
    if (direction == MovementDirection::Up && movementFlags & Movement::AllowUp)
    {
        this->direction = direction;
        state = MovementState::Walking;
        sprite.move({0.f, -maxVelocity * sprite.getScale().x * dt});
    }
    else if (direction == MovementDirection::Down && movementFlags & Movement::AllowDown)
    {
        this->direction = direction;
        state = MovementState::Walking;
        sprite.move({0.f, maxVelocity * sprite.getScale().x * dt});
    }
    else if (direction == MovementDirection::Left && movementFlags & Movement::AllowLeft)
    {
        this->direction = direction;
        state = MovementState::Walking;
        sprite.move({-maxVelocity * sprite.getScale().x * dt, 0.f});
    }
    else if (direction == MovementDirection::Right && movementFlags & Movement::AllowRight)
    {
        this->direction = direction;
        state = MovementState::Walking;
        sprite.move({maxVelocity * sprite.getScale().x * dt, 0.f});
    }
}

const MovementState &MovementFunctionality::getState() const
{
    return state;
}

const MovementDirection &MovementFunctionality::getDirection() const
{
    return direction;
}

const std::string MovementFunctionality::getDirectionAsString() const
{
    std::string str;

    switch (direction)
    {
    case Up:
        str = "Up";
        break;
    case Down:
        str = "Down";
        break;
    case Left:
        str = "Left";
        break;
    case Right:
        str = "Right";
        break;
    }

    return str;
}
