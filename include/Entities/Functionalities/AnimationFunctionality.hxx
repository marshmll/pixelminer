/**
 * @file AnimationFunctionality.hxx
 * @brief Defines the AnimationFunctionality class for managing entity animations.
 */

#pragma once

#include "Animations/Animation.hxx"
#include "Tools/Logger.hxx"

/**
 * @class AnimationFunctionality
 * @brief Handles animations for entities, managing different animation states.
 */
class AnimationFunctionality
{
  private:
    Logger logger; ///< Logger instance for debugging and information logging.

    std::string entityName;                                     ///< Name of the entity associated with the animations.
    std::uint64_t entityId;                                     ///< Unique identifier for the entity.
    std::map<std::string, std::shared_ptr<sf::Sprite>> &layers; ///< Reference to the entity's sprite layers.
    sf::Texture &spriteSheet;                                   ///< Reference to the entity's sprite sheet.

    std::map<std::string, std::unique_ptr<Animation>> animations; ///< Map of animations by name.

  public:
    /**
     * @brief Constructs an AnimationFunctionality instance.
     * @param entity_name Name of the entity.
     * @param entity_id Unique identifier of the entity.
     * @param layers Reference to the sprite layers of the entity.
     * @param sprite_sheet Reference to the sprite sheet used for animations.
     */
    AnimationFunctionality(const std::string &entity_name, const std::uint64_t entity_id,
                           std::map<std::string, std::shared_ptr<sf::Sprite>> &layers, sf::Texture &sprite_sheet);

    /**
     * @brief Destructor for AnimationFunctionality.
     */
    ~AnimationFunctionality();

    /**
     * @brief Adds a new animation to a specific layer.
     * @param layer The layer to which the animation belongs.
     * @param name Name of the animation.
     * @param frametime_as_ms Frame time in milliseconds.
     * @param frame_size Size of each frame in pixels.
     * @param start_frame_index Index of the starting frame.
     * @param end_frame_index Index of the ending frame.
     * @param boomerang Whether the animation plays in reverse after completing.
     */
    void addAnimation(const std::string &layer, const std::string name, const std::int32_t &frametime_as_ms,
                      const sf::Vector2u &frame_size, const sf::Vector2u &start_frame_index,
                      const sf::Vector2u &end_frame_index, const bool &boomerang = false);

    /**
     * @brief Plays the specified animation.
     * @param name Name of the animation to play.
     * @param reset_others Whether to reset other animations.
     */
    void play(const std::string &name, const bool reset_others = false);

    /**
     * @brief Resets the specified animation.
     * @param name Name of the animation to reset.
     */
    void reset(const std::string &name);
};
