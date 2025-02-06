#pragma once

#include "Engine/Configuration.hxx"

struct HitBox
{
    sf::RectangleShape rect;
    sf::Vector2f offset;
};

class CollisionFunctionality
{
  private:
    sf::Sprite &sprite;
    std::unordered_map<std::string, HitBox> hitBoxes;
    HitBox predictionHitBox;
    bool collisionEnabled;

  public:
    CollisionFunctionality(sf::Sprite &sprite);

    ~CollisionFunctionality();

    void update();

    const std::unordered_map<std::string, HitBox> &getHitBoxes() const;

    const HitBox getHitBox(const std::string &key) const;

    const HitBox &predictHitBoxPosition(const std::string key, const sf::Vector2f &velocity);

    const bool &getCollisionEnabled() const;

    void addHitBox(const std::string &key, const sf::Vector2u &size_in_pixels, const sf::Vector2u &offset_in_pixels,
                   const float &scale);

    void setCollisionEnabled(const bool &enabled);
};