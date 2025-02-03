#include "Entities/Entity.hxx"
#include "stdafx.hxx"

void Entity::createMovementFunctionality(const float &max_velocity, const uint8_t &movement_flags,
                                         const uint8_t &movement_direction, const uint8_t &movement_state)
{
    movementFunctionality.emplace(layers, max_velocity, movement_flags, movement_direction, movement_state);
}

void Entity::createAnimationFunctionality()
{
    animationFunctionality.emplace(name, id, layers, spriteSheet);
}

void Entity::createAttributeFunctionality(const uint8_t &max_health, const uint8_t &max_hunger)
{
    attributeFunctionality.emplace(max_health, max_hunger);
}

Entity::Entity(const std::string name, const sf::Vector2f spawn_grid_position, sf::Texture &baseSprite_sheet,
               const float &scale)

    : name(name), spawnGridPosition(spawn_grid_position), id(reinterpret_cast<uint64_t>(this)),
      spriteSheet(baseSprite_sheet), scale(scale)
{
    addSpriteLayer("Base");
    baseSprite = layers.at("Base");
    baseSprite->setPosition(spawnGridPosition * static_cast<float>(GRID_SIZE * scale));
}

Entity::~Entity()
{
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
    return baseSprite->getPosition();
}

std::shared_ptr<sf::Sprite> Entity::getSpriteLayer(const std::string &key)
{
    if (layers.count(key) == 0)
        return nullptr;

    return layers.at(key);
}

const sf::Vector2i Entity::getGridPosition() const
{
    return sf::Vector2i(static_cast<int>(baseSprite->getPosition().x / (GRID_SIZE * baseSprite->getScale().x)),
                        static_cast<int>(baseSprite->getPosition().y / (GRID_SIZE * baseSprite->getScale().y)));
}

const sf::Vector2f Entity::getCenter() const
{
    return sf::Vector2f(baseSprite->getPosition().x + baseSprite->getGlobalBounds().size.x / 2.f,
                        baseSprite->getPosition().y + baseSprite->getGlobalBounds().size.y / 2.f);
}

const sf::Vector2i Entity::getCenterGridPosition() const
{
    return sf::Vector2i(static_cast<int>(getCenter().x / (GRID_SIZE * baseSprite->getScale().x)),
                        static_cast<int>(getCenter().y / (GRID_SIZE * baseSprite->getScale().y)));
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
    sf::Vector2f offset = baseSprite->getPosition() - position; // Offset to move each sprite layer proportionally

    for (auto &[_, sprite] : layers)
    {
        if (sprite)
            sprite->move(offset);
    }
}

void Entity::addSpriteLayer(const std::string &key)
{
    layers[key] = std::make_unique<sf::Sprite>(spriteSheet);
    layers.at(key)->setScale({scale, scale});

    if (baseSprite)
        layers.at(key)->setPosition(baseSprite->getPosition());
}