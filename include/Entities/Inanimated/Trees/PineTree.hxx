/**
 * @file PineTree.hxx
 * @brief Declares the PineTree class for the Pine Tree variant.
 */

#pragma once

#include "Entities/Inanimated/Trees/Tree.hxx"

/**
 * @class PineTree
 * @brief Represents a Pine tree in the game.
 *
 * Inherits from the Tree class and handles specific logic for a Pine tree, including
 * animations, collision, and rendering.
 */
class PineTree : public Tree
{
  private:
    /**
     * @brief Initializes the PineTree animations.
     *
     * Sets up the crown and trunk animations.
     */
    void initAnimations();

    /**
     * @brief Initializes the PineTree hitboxes.
     *
     * Sets up the collision hitbox for the trunk.
     */
    void initHitBoxes();

  public:
    /**
     * @brief Constructs a PineTree object.
     *
     * Initializes the tree with position, sprite sheet, and scale.
     *
     * @param spawn_grid_position Position of the tree.
     * @param sprite_sheet Texture for rendering.
     * @param scale Scale of the tree.
     */
    PineTree(const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet, const float &scale, std::unordered_map<std::string, sf::SoundBuffer> &sound_buffers);

    /**
     * @brief Destructor for PineTree.
     *
     * Destroys the PineTree object.
     */
    ~PineTree();

    /**
     * @brief Updates the PineTree state.
     *
     * Plays animations and updates collision.
     *
     * @param dt Time elapsed since last frame.
     * @param mouse_pos Current mouse position.
     */
    void update(const float &dt, const sf::Vector2f &mouse_pos);

    /**
     * @brief Renders the PineTree.
     *
     * Draws the tree's sprite layers.
     *
     * @param target Render target.
     */
    void render(sf::RenderTarget &target);

    /**
     * @brief Renders the PineTree with hitboxes.
     *
     * Draws the tree and optionally shows hitboxes.
     *
     * @param target Render target.
     * @param show_hitboxes Flag to show hitboxes.
     */
    void render(sf::RenderTarget &target, const bool &show_hitboxes);
};
