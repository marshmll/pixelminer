#include "Entities/Entity.hxx"
#include "stdafx.hxx"

void Entity::createMovementFunctionality(const float &max_velocity, const std::uint8_t &movement_flags)
{
    movementFunctionality.emplace(sprite, max_velocity, movement_flags);
}

Entity::Entity(const std::string name, const sf::Vector2f spawn_position, sf::Texture &sprite_sheet, const float &scale)
    : name(name), spawnPosition(spawn_position), id(reinterpret_cast<std::uint64_t>(this)), spriteSheet(sprite_sheet),
      sprite(spriteSheet)
{
    sprite.setPosition(spawnPosition);
    sprite.setScale({scale, scale});
}

Entity::~Entity()
{
}

void Entity::update(const float &dt)
{
    
}

void Entity::render(sf::RenderTarget &target)
{
    target.draw(sprite);
}

void Entity::move(const float &dt, const MovementDirection &direction)
{
    if (movementFunctionality.has_value())
        movementFunctionality->move(dt, direction);
    else
        std::cout << "[ Entity: \"" << name << "\" ID: " << std::hex << id << std::dec
                  << " ]: Tried to move without an initialized movement component." << "\n";
}