#include "Entities/Entity.hxx"
#include "stdafx.hxx"

void Entity::createMovementFunctionality(const float &max_velocity, const uint8_t &movement_flags,
                                         const uint8_t &movement_direction, const uint8_t &movement_state)
{
    movementFunctionality.emplace(layers, max_velocity, scale, movement_flags, movement_direction, movement_state);
}

void Entity::createAnimationFunctionality()
{
    animationFunctionality.emplace(name, id, layers, spriteSheet);
}

void Entity::createAttributeFunctionality(const uint8_t &max_health, const uint8_t &max_hunger)
{
    attributeFunctionality.emplace(max_health, max_hunger);
}

void Entity::createCollisionFunctionality()
{
    collisionFunctionality.emplace(*baseSprite);
}

Entity::Entity(const std::string name, const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet,
               const float &scale)

    : logger(name), name(name), spawnGridPosition(spawn_grid_position), id(reinterpret_cast<uint64_t>(this)),
      spriteSheet(sprite_sheet), scale(scale)
{
    addSpriteLayer("Base");
    baseSprite = layers.at("Base");
    baseSprite->setPosition(
        sf::Vector2f(spawnGridPosition * static_cast<float>(GRID_SIZE) * scale) +
        sf::Vector2f(baseSprite->getGlobalBounds().size.x / 2.f, baseSprite->getGlobalBounds().size.y / 2.f));
}

Entity::~Entity()
{
}

void Entity::move(const float &dt, const MovementDirection &direction)
{
    if (movementFunctionality.has_value())
        movementFunctionality->move(dt, direction);
    else
        logger.logWarning("Entity with ID: " + std::to_string(id) +
                          " tried to move without an initialized movement component.");
}

void Entity::playAnimation(const std::string &name)
{
    if (animationFunctionality.has_value())
        animationFunctionality->play(name);

    else
        logger.logWarning("Entity with ID: " + std::to_string(id) +
                          " tried to play an animation without an initialized animation component.");
}

const std::string &Entity::getName() const
{
    return name;
}

const uint64_t &Entity::getId() const
{
    return id;
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

const sf::Vector2f Entity::getGridPosition() const
{
    return sf::Vector2f((baseSprite->getPosition().x / static_cast<float>(GRID_SIZE * scale)) * 100 / 100,
                        (baseSprite->getPosition().y / static_cast<float>(GRID_SIZE * scale)) * 100 / 100);
}

const sf::Vector2f Entity::getCenter() const
{
    return sf::Vector2f((baseSprite->getPosition().x + baseSprite->getGlobalBounds().size.x / 2.f) * 100 / 100,
                        (baseSprite->getPosition().y + baseSprite->getGlobalBounds().size.y / 2.f) * 100 / 100);
}

const sf::Vector2f Entity::getCenterGridPosition() const
{
    return sf::Vector2f((getCenter().x / static_cast<float>(GRID_SIZE * scale)) * 100 / 100,
                        (getCenter().y / static_cast<float>(GRID_SIZE * scale)) * 100 / 100);
}

AttributeFunctionality &Entity::getAttributeFunctionality()
{
    if (!attributeFunctionality.has_value())
        logger.logError("Entity " + name + " ID: " + std::to_string(id) +
                        " accessed non-initialized AttributeFunctionality.");

    return *attributeFunctionality;
}

void Entity::setPosition(const sf::Vector2f &position)
{
    sf::Vector2f previous_pos = baseSprite->getPosition();
    sf::Vector2f offset((position.x - previous_pos.x) * 100.f / 100.f, (position.y - previous_pos.y) * 100.f / 100.f);

    for (auto &[_, sprite] : layers)
        if (sprite)
            sprite->move(offset);
}

void Entity::setGridPosition(const sf::Vector2f &grid_position)
{
    sf::Vector2f previous_pos = baseSprite->getPosition();
    sf::Vector2f new_pos(sf::Vector2f(grid_position.x * GRID_SIZE * scale, grid_position.y * GRID_SIZE * scale));
    sf::Vector2f offset(std::floor(new_pos.x - previous_pos.x), std::floor(new_pos.y - previous_pos.y));

    for (auto &[_, sprite] : layers)
        if (sprite)
            sprite->move(offset);
}

void Entity::setCenterGridPosition(const sf::Vector2f &grid_position)
{
    sf::Vector2f previous_pos(baseSprite->getPosition() + baseSprite->getGlobalBounds().size / 2.f);
    sf::Vector2f new_pos(sf::Vector2f(grid_position.x * GRID_SIZE * scale, grid_position.y * GRID_SIZE * scale));
    sf::Vector2f offset(std::floor(new_pos.x - previous_pos.x), std::floor(new_pos.y - previous_pos.y));

    for (auto &[_, sprite] : layers)
        if (sprite)
            sprite->move(offset);
}

void Entity::addSpriteLayer(const std::string &key)
{
    layers[key] = std::make_unique<sf::Sprite>(spriteSheet);
    layers.at(key)->setScale({scale, scale});

    if (baseSprite)
        layers.at(key)->setPosition(baseSprite->getPosition());
}

const std::unordered_map<std::string, HitBox> &Entity::getHitBoxes()
{
    if (!collisionFunctionality.has_value())
        logger.logError("Entity " + name + " does not have a collision functionality.");

    return collisionFunctionality->getHitBoxes();
}

const HitBox Entity::getHitBox(const std::string &key) const
{
    return collisionFunctionality->getHitBox(key);
}