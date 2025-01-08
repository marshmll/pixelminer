#include "Entities/Entity.hxx"
#include "stdafx.hxx"

void Entity::createMovementAbility(const float &max_velocity, const std::uint8_t &movement_flags)
{
    movementAbility.emplace(sprite, max_velocity, movement_flags);
}

Entity::Entity(const std::string name, const sf::Vector2f position)
    : name(name), id(reinterpret_cast<std::uint64_t>(this)), texture("Assets/Images/Backgrounds/stone.png"),
      sprite(texture)
{
    createMovementAbility(500.f, Movement::AllowLeft | Movement::AllowRight);
}

Entity::~Entity()
{
}

void Entity::update(const float &dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        move(dt, Up);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        move(dt, Down);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        move(dt, Left);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        move(dt, Right);
}

void Entity::render(sf::RenderTarget &target)
{
    target.draw(sprite);
}

void Entity::move(const float &dt, const MovementDirection &direction)
{
    if (movementAbility.has_value())
        movementAbility->move(dt, direction);
    else
        std::cout << "[ Entity: \"" << name << "\" ID: " << std::hex << id << std::dec
                  << " ]: Tried to move without an initialized movement component." << "\n";
}