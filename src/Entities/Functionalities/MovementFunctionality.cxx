#include "Entities/Functionalities/MovementFunctionality.hxx"
#include "stdafx.hxx"

MovementFunctionality::MovementFunctionality(sf::Sprite &sprite, const float max_velocity, const std::uint8_t movement_flags)
    : sprite(sprite), maxVelocity(max_velocity), movementFlags(movement_flags)
{
}

MovementFunctionality::~MovementFunctionality()
{
}

void MovementFunctionality::move(const float &dt, const MovementDirection direction)
{
    state = MovementState::Idle;

    if (direction == MovementDirection::Up && movementFlags & Movement::AllowUp)
    {
        this->direction = direction;
        state = MovementState::Moving;
        sprite.move({0.f, -maxVelocity * sprite.getScale().x * dt});
    }
    else if (direction == MovementDirection::Down && movementFlags & Movement::AllowDown)
    {
        this->direction = direction;
        state = MovementState::Moving;
        sprite.move({0.f, maxVelocity * sprite.getScale().x * dt});
    }
    else if (direction == MovementDirection::Left && movementFlags & Movement::AllowLeft)
    {
        this->direction = direction;
        state = MovementState::Moving;
        sprite.move({-maxVelocity * sprite.getScale().x * dt, 0.f});
    }
    else if (direction == MovementDirection::Right && movementFlags & Movement::AllowRight)
    {
        this->direction = direction;
        state = MovementState::Moving;
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