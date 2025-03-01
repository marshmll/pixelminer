/**
 * @file TileBase.hxx
 * @brief Declares the TileBase class that establishes a base for any tile. This is an abstract class.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "TileData.hxx"

constexpr int QUAD_VERTEX_COUNT = 6;

/**
 * @class TileBase
 * @brief A base class representing a tile in the game world.
 *
 * This class provides common properties and methods for handling tiles, such as
 * texture, position, grid positioning, and rendering. The tile is drawn as a sprite
 * with a specified texture and can be manipulated using the available methods.
 */
class TileBase : public sf::Drawable, public sf::Transformable
{
  private:
    /**
     * @brief Draws the tile into a render target.
     *
     * @param target The target to which the tile will be rendered.
     * @param states The render states.
     */
    void draw(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const override;

  protected:
    std::string name;         ///< The name of the tile.
    std::string tag;          ///< The tag of the tile in the format `domain:name`.
    uint64_t id;              ///< The unique identifier of the tile.
    sf::Texture &texture;     ///< The texture used for rendering the tile.
    sf::IntRect textureRect;  ///< The portion of the texture to use for the tile.
    sf::VertexArray vertices; ///< The render vertices of the tile.
    float scaleScalar;        ///< The scale used in the game.

  public:
    /**
     * @brief Construct a new TileBase object.
     *
     * @param name The name of the tile.
     * @param tag The tag of the tile, in the format `domain:name`.
     * @param id The unique identifier of the tile.
     * @param texture The texture to be used for the tile.
     * @param texture_rect The portion of the texture to be used.
     * @param grid_position The grid position of the tile
     * @param scale The scale factor for the tile (default is 1.f).
     */
    TileBase(const std::string &name, const std::string &tag, const uint64_t &id, sf::Texture &texture,
             const sf::IntRect &texture_rect, const sf::Vector2i &grid_position, const float &scale = 1.f);

    /**
     * @brief Destroy the TileBase object.
     */
    virtual ~TileBase();

    /**
     * @brief Get the name of the tile.
     *
     * @return The name of the tile.
     */
    const std::string &getName() const;

    /**
     * @brief Get the unique tag of the tile.
     *
     * @return The unique tag of the tile.
     */
    const std::string &getTag() const;

    /**
     * @brief Get the unique id of the tile.
     *
     * @return The unique id of the tile.
     */
    const uint64_t &getId() const;

    /**
     * @brief Get the position of the tile.
     *
     * @return The position of the tile.
     */
    const sf::Vector2f getPosition() const;

    /**
     * @brief Get the grid position of the tile (in grid units).
     *
     * @return The grid position of the tile.
     */
    const sf::Vector2u getGridPosition() const;

    /**
     * @brief Get the color of the tile.
     *
     * @return color of the tile.
     */
    sf::Color getColor() const;

    /**
     * @brief Get the global bounds of the tile.
     *
     * @return The global bounds of the tile.
     */
    const sf::FloatRect getGlobalBounds() const;

    /**
     * @brief Get the center position of the tile.
     *
     * @return The center position of the tile.
     */
    const sf::Vector2f getCenter() const;

    /**
     * @brief Returns the `sf::VertexArray` reference of the tile.
     *
     * @return The `sf::VertexArray` reference of the tile.
     */
    const sf::VertexArray &getVertexArray() const;

    /**
     * @brief Set the position of the tile.
     *
     * @param position The new position for the tile.
     */
    void setPosition(const sf::Vector2f &position);

    /**
     * @brief Set the grid position of the tile (in grid units).
     *
     * @param position The new grid position for the tile.
     */
    void setGridPosition(const sf::Vector2u &position);

    /**
     * @brief Set the color of the tile.
     *
     * @param color The new color for the tile.
     */
    void setColor(const sf::Color &color);
};
