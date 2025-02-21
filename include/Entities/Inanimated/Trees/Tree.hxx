/**
 * @file Tree.hxx
 * @brief Declares the Tree base class for trees.
 */

#pragma once

#include "Entities/Entity.hxx"

/**
 * @class Tree
 * @brief Represents a tree entity in the game. Inherits from Entity and provides basic functionality for trees.
 */
class Tree : public Entity
{
  protected:
  public:
    /**
     * @brief Constructs a Tree object.
     * @param name Name of the tree.
     * @param spawn_grid_position Spawn position of the tree in grid coordinates.
     * @param sprite_sheet Reference to the sprite sheet texture.
     * @param scale Scaling factor for the tree's sprites.
     */
    Tree(const std::string name, const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet, const float &scale, std::unordered_map<std::string, sf::SoundBuffer> &sound_buffers);

    /**
     * @brief Destructor for the Tree class.
     */
    virtual ~Tree();

    /**
     * @brief Updates the tree's state.
     * @param dt Delta time for frame-independent updates.
     * @param mouse_pos Current position of the mouse.
     */
    virtual void update(const float &dt, const sf::Vector2f &mouse_pos) = 0;

    /**
     * @brief Renders the tree on the target.
     * @param target Render target to draw the tree on.
     */
    virtual void render(sf::RenderTarget &target) = 0;

    /**
     * @brief Renders the tree on the target, optionally showing hitboxes.
     * @param target Render target to draw the tree on.
     * @param show_hitboxes Whether to render hitboxes.
     */
    virtual void render(sf::RenderTarget &target, const bool &show_hitboxes) = 0;
};