#include "Entities/Entity.hxx"
#include "stdafx.hxx"

void Entity::createMovementFunctionality(const float &max_velocity, const uint8_t &movement_flags,
                                         const uint8_t &movement_direction, const uint8_t &movement_state)
{
    movementFunctionality.emplace(sprite, max_velocity, movement_flags, movement_direction, movement_state);
}

void Entity::createAnimationFunctionality()
{
    animationFunctionality.emplace(name, id, sprite, spriteSheet);
}

void Entity::createAttributeFunctionality(const uint8_t &max_health, const uint8_t &max_hunger)
{
    attributeFunctionality.emplace(max_health, max_hunger);
}

Entity::Entity(const std::string name, const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet,
               const float &scale)

    : name(name), spawnGridPosition(spawn_grid_position), id(reinterpret_cast<uint64_t>(this)),
      spriteSheet(sprite_sheet), sprite(spriteSheet)
{
    sprite.setScale({scale, scale});
    sprite.setPosition(spawnGridPosition * static_cast<float>(GRID_SIZE * scale));
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

void Entity::playAnimation(const std::string &name)
{
    if (animationFunctionality.has_value())
        animationFunctionality->play(name);

    else
        std::cout << "[ Entity: \"" << name << "\" ID: " << std::hex << id << std::dec
                  << " ]: Tried to play animation without an initialized animation component." << "\n";
}

const sf::Vector2f Entity::getPosition() const
{
    return sprite.getPosition();
}

const sf::Vector2i Entity::getGridPosition() const
{
    return sf::Vector2i(static_cast<int>(sprite.getPosition().x / (GRID_SIZE * sprite.getScale().x)),
                        static_cast<int>(sprite.getPosition().y / (GRID_SIZE * sprite.getScale().y)));
}

const sf::Vector2f Entity::getCenter() const
{
    return sf::Vector2f(sprite.getPosition().x + sprite.getGlobalBounds().size.x / 2.f,
                        sprite.getPosition().y + sprite.getGlobalBounds().size.y / 2.f);
}

const sf::Vector2i Entity::getCenterGridPosition() const
{
    return sf::Vector2i(static_cast<int>(getCenter().x / (GRID_SIZE * sprite.getScale().x)),
                        static_cast<int>(getCenter().y / (GRID_SIZE * sprite.getScale().y)));
}

AttributeFunctionality &Entity::getAttributeFunctionality()
{
    if (!attributeFunctionality.has_value())
        throw std::runtime_error("Entity " + name + " ID: " + std::to_string(id) +
                                 " accessed non-initialized AttributeFunctionality.");

    return *attributeFunctionality;
}

void Entity::setPosition(const sf::Vector2f &position)
{
    sprite.setPosition(position);
}