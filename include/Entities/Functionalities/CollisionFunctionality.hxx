/**
 * @file CollisionFunctionality.hxx
 * @brief Declares the CollisionFunctionality class for managing entity collisions.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "Tools/Logger.hxx"

/**
 * @class HitBox
 * @brief Represents the collideable part of an entity.
 */
class HitBox
{
  public:
    sf::RectangleShape rect; ///< The actual rectangle shape of the hitbox.
    sf::Vector2f offset;     ///< The (scaled) offset of the hitbox relative to a sprite.

    /**
     * @brief Predicts the next instance of the hitbox based on a velocity.
     * @param velocity The velocity to be used in the prediction.
     * @return The predicted hitbox.
     */
    inline HitBox predictNextPos(const sf::Vector2f &velocity)
    {
        HitBox hb = *this;
        hb.rect.move(velocity);
        return hb;
    }

    /**
     * @brief Finds the intersection rect between two hitboxes.
     * @param hitbox The other hitbox to check intersection.
     * @return The intersection rect if exists, std::nullopt otherwise.
     */
    inline std::optional<sf::FloatRect> findIntersection(HitBox &hitbox)
    {
        return rect.getGlobalBounds().findIntersection(hitbox.rect.getGlobalBounds());
    }
};

/**
 * @class CollisionFunctionality
 * @brief Handles hitboxes and collision for an entity.
 */
class CollisionFunctionality
{
  private:
    Logger logger;                          ///< Logger instance for printing logs.
    sf::Sprite &sprite;                     ///< The entity's sprite reference.
    std::map<std::string, HitBox> hitBoxes; ///< Collecion of hitboxes owned by the entity.
    HitBox predictionHitBox;                ///< Buffer for hitbox predictions.
    bool collisionEnabled;                  ///< Flag to indicate if the collision is enabled.

  public:
    /**
     * @brief Constructs an CollisionFunctionality instance.
     * @param sprite The entity's sprite reference.
     */
    CollisionFunctionality(sf::Sprite &sprite);

    /**
     * @brief Destructor for AttributeFunctionality.
     */
    ~CollisionFunctionality();

    /**
     * @brief Update all hitboxes positions if the sprite position changes.
     */
    void update();

    /**
     * @brief Non-const accessor for the hitboxes collection.
     * @return The collection of hitboxes.
     */
    std::map<std::string, HitBox> &getHitBoxes();

    /**
     * @brief Non-const accessor for the first hitbox inserted in the hitbox collection.
     * @return The first hitbox if exists, a default constructed hitbox otherwise.
     */
    HitBox &getFirstHitBox();

    /**
     * @brief Accessor for the hitboxes in the hitbox collection.
     * @param key The key of the hitbox in the collection.
     * @return The hitbox if the key matches any, a default constructed hitbox otherwise.
     */
    const HitBox getHitBox(const std::string &key) const;

    /**
     * @brief Accessor for collision enabled flag.
     * @return True if the collision is enabled, false otherwise.
     */
    const bool &getCollisionEnabled() const;

    /**
     * @brief Adds or replaces a hitbox in the hitbox collecion.
     * @param key The key of the new hitbox.
     * @param size_in_pixels The size of the hitbox, disconsidering any scaling.
     * @param offset_in_pixels The offset of the hitbox relative to the sprite, disconsidering any scaling.
     * @param scale The same scale used by the sprite.
     */
    void addHitBox(const std::string &key, const sf::Vector2u &size_in_pixels, const sf::Vector2u &offset_in_pixels,
                   const float &scale);

    /**
     * @brief Modifier for collision enabled flag.
     * @param enabled The new value for the collision enabled flag.
     */
    void setCollisionEnabled(const bool &enabled);
};