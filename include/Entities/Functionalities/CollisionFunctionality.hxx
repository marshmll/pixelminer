#pragma once

#include "Engine/Configuration.hxx"
#include "Tools/Logger.hxx"

class HitBox
{
  public:
    sf::RectangleShape rect;
    sf::Vector2f offset;

    inline HitBox predictNextPos(const sf::Vector2f &velocity)
    {
        HitBox hb = *this;
        hb.rect.move(velocity);
        return hb;
    }

    std::optional<sf::FloatRect> findIntersection(HitBox &hitbox)
    {
        return rect.getGlobalBounds().findIntersection(hitbox.rect.getGlobalBounds());
    }
};

class CollisionFunctionality
{
  private:
    Logger logger;
    sf::Sprite &sprite;
    std::map<std::string, HitBox> hitBoxes;
    HitBox predictionHitBox;
    bool collisionEnabled;

  public:
    CollisionFunctionality(sf::Sprite &sprite);

    ~CollisionFunctionality();

    void update();

    std::map<std::string, HitBox> &getHitBoxes();

    HitBox &getFirstHitBox();

    const HitBox getHitBox(const std::string &key) const;

    const HitBox &predictHitBoxPosition(const std::string key, const sf::Vector2f &velocity);

    const bool &getCollisionEnabled() const;

    void addHitBox(const std::string &key, const sf::Vector2u &size_in_pixels, const sf::Vector2u &offset_in_pixels,
                   const float &scale);

    void setCollisionEnabled(const bool &enabled);
};