#include "Entities/Abilities/MovementAbility.hxx"
#include "stdafx.hxx"

MovementAbility::MovementAbility(sf::Sprite &sprite, const float max_velocity, const std::uint8_t movement_flags)
    : sprite(sprite), maxVelocity(max_velocity), movementFlags(movement_flags)
{
}

MovementAbility::~MovementAbility()
{
}

void MovementAbility::move(const float &dt, const MovementDirection direction)
{
    state = MovementState::Idle;

    if (direction == MovementDirection::Up && movementFlags & Movement::AllowUp)
    {
        this->direction = direction;
        state = MovementState::Moving;
        sprite.move({0.f, -maxVelocity * dt});
    }
    else if (direction == MovementDirection::Down && movementFlags & Movement::AllowDown)
    {
        this->direction = direction;
        state = MovementState::Moving;
        sprite.move({0.f, maxVelocity * dt});
    }
    else if (direction == MovementDirection::Left && movementFlags & Movement::AllowLeft)
    {
        this->direction = direction;
        state = MovementState::Moving;
        sprite.move({-maxVelocity * dt, 0.f});
    }
    else if (direction == MovementDirection::Right && movementFlags & Movement::AllowRight)
    {
        this->direction = direction;
        state = MovementState::Moving;
        sprite.move({maxVelocity * dt, 0.f});
    }
}

const MovementState &MovementAbility::getState() const
{
    return state;
}

const MovementDirection &MovementAbility::getDirection() const
{
    return direction;
}