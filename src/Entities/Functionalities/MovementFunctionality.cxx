#include "Entities/Functionalities/MovementFunctionality.hxx"
#include "stdafx.hxx"

MovementFunctionality::MovementFunctionality(std::map<std::string, std::shared_ptr<sf::Sprite>> &layers,
                                             const float max_velocity, const uint8_t &movement_flags,
                                             const uint8_t &movement_direction, const uint8_t &movement_state)
    : layers(layers), maxVelocity(max_velocity), flags(movement_flags), state(movement_state),
      direction(movement_direction)
{
}

MovementFunctionality::~MovementFunctionality()
{
}

void MovementFunctionality::update()
{
    state = MovementState::Idle;
}

void MovementFunctionality::move(const float &dt, const uint8_t direction)
{
    if (direction == MovementDirection::Up && flags & Movement::AllowUp)
    {
        this->direction = direction;
        state = MovementState::Walking;

        for (auto &[_, sprite] : layers)
        {
            if (sprite)
                sprite->move(sf::Vector2f(0.f, std::round(-maxVelocity * sprite->getScale().y * dt)));
        }
    }
    else if (direction == MovementDirection::Down && flags & Movement::AllowDown)
    {
        this->direction = direction;
        state = MovementState::Walking;

        for (auto &[_, sprite] : layers)
        {
            if (sprite)
                sprite->move(sf::Vector2f(0.f, std::round(maxVelocity * sprite->getScale().y * dt)));
        }
    }
    else if (direction == MovementDirection::Left && flags & Movement::AllowLeft)
    {
        this->direction = direction;
        state = MovementState::Walking;

        for (auto &[_, sprite] : layers)
        {
            if (sprite)
                sprite->move(sf::Vector2f(std::round(-maxVelocity * sprite->getScale().x) * dt, 0.f));
        }
    }
    else if (direction == MovementDirection::Right && flags & Movement::AllowRight)
    {
        this->direction = direction;
        state = MovementState::Walking;

        for (auto &[_, sprite] : layers)
        {
            if (sprite)
                sprite->move(sf::Vector2f(std::round(maxVelocity * sprite->getScale().x * dt), 0.f));
        }
    }
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
