#include "Entities/Functionalities/CollisionFunctionality.hxx"
#include "stdafx.hxx"

CollisionFunctionality::CollisionFunctionality(sf::Sprite &sprite) : sprite(sprite), collisionEnabled(true)
{
}

CollisionFunctionality::~CollisionFunctionality() = default;

void CollisionFunctionality::update()
{
    for (auto &[_, hitbox] : hitBoxes)
        hitbox.rect.setPosition(sprite.getPosition() + hitbox.offset);
}

const std::unordered_map<std::string, HitBox> &CollisionFunctionality::getHitBoxes() const
{
    return hitBoxes;
}

const HitBox CollisionFunctionality::getHitBox(const std::string &key) const
{
    try
    {
        return hitBoxes.at(key);
    }
    catch (std::out_of_range &)
    {
        return HitBox();
    }
}

const HitBox &CollisionFunctionality::predictHitBoxPosition(const std::string key, const sf::Vector2f &velocity)
{
    try
    {
        predictionHitBox = hitBoxes.at(key);
        predictionHitBox.rect.move(velocity);
        return predictionHitBox;
    }
    catch (std::out_of_range &)
    {
        predictionHitBox = HitBox();
        return predictionHitBox;
    }
}

const bool &CollisionFunctionality::getCollisionEnabled() const
{
    return collisionEnabled;
}

void CollisionFunctionality::addHitBox(const std::string &key, const sf::Vector2u &size_in_pixels,
                                       const sf::Vector2u &offset_in_pixels, const float &scale)
{
    HitBox hitbox;

    hitbox.rect.setSize(sf::Vector2f(size_in_pixels.x * scale, size_in_pixels.y * scale));
    hitbox.rect.setPosition(sprite.getPosition() +
                            sf::Vector2f(offset_in_pixels.x * scale, offset_in_pixels.y * scale));
    hitbox.rect.setFillColor(sf::Color::Transparent);
    hitbox.rect.setOutlineThickness(1.f);
    hitbox.rect.setOutlineColor(sf::Color::Red);
    hitbox.offset = sf::Vector2f(offset_in_pixels.x * scale, offset_in_pixels.y * scale);

    hitBoxes[key] = hitbox;
}

void CollisionFunctionality::setCollisionEnabled(const bool &enabled)
{
    collisionEnabled = enabled;
}
