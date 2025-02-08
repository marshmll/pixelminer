/**
 * @file TileBase.hxx
 * @brief Declares the TileBase class that establishes a base for any tile. This is an abstract class.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "TileData.hxx"

/**
 * @class TileBase
 * @brief A base class representing a tile in the game world.
 *
 * This class provides common properties and methods for handling tiles, such as
 * texture, position, grid positioning, and rendering. The tile is drawn as a sprite
 * with a specified texture and can be manipulated using the available methods.
 */
class TileBase
{
  protected:
    std::string name;        ///< The name of the tile.
    std::string id;          ///< The unique identifier of the tile.
    sf::Texture &texture;    ///< The texture used for rendering the tile.
    sf::IntRect textureRect; ///< The portion of the texture to use for the tile.
    sf::Sprite sprite;       ///< The sprite used to render the tile.

  public:
    /**
     * @brief Construct a new TileBase object.
     *
     * @param name The name of the tile.
     * @param id The unique identifier of the tile.
     * @param texture The texture to be used for the tile.
     * @param texture_rect The portion of the texture to be used.
     * @param scale The scale factor for the tile (default is 1).
     */
    TileBase(const std::string name, const std::string &id, sf::Texture &texture, const sf::IntRect &texture_rect,
             const unsigned int &scale = 1);

    /**
     * @brief Destroy the TileBase object.
     */
    virtual ~TileBase();

    /**
     * @brief Renders the tile to the given render target.
     *
     * @param target The target to which the tile will be rendered.
     */
    virtual void render(sf::RenderTarget &target);

    /**
     * @brief Get the unique identifier of the tile.
     *
     * @return const std::string& The unique identifier of the tile.
     */
    const std::string &getId() const;

    /**
     * @brief Get the name of the tile.
     *
     * @return const std::string& The name of the tile.
     */
    const std::string &getName() const;

    /**
     * @brief Get the position of the tile.
     *
     * @return const sf::Vector2f The position of the tile.
     */
    const sf::Vector2f getPosition() const;

    /**
     * @brief Get the grid position of the tile (in grid units).
     *
     * @return const sf::Vector2u The grid position of the tile.
     */
    const sf::Vector2u getGridPosition() const;

    /**
     * @brief Get the color of the tile.
     *
     * @return sf::Color The color of the tile.
     */
    sf::Color getColor() const;

    /**
     * @brief Get the global bounds of the tile.
     *
     * @return const sf::FloatRect The global bounds of the tile.
     */
    const sf::FloatRect getGlobalBounds() const;

    /**
     * @brief Get the center position of the tile.
     *
     * @return const sf::Vector2f The center position of the tile.
     */
    const sf::Vector2f getCenter() const;

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
