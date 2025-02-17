/**
 * @file Tile.hxx
 * @brief Declares the Tile class for representing any kind of tile that can be rendered in a map grid.
 */

#pragma once

#include "Tiles/TileBase.hxx"

/**
 * @class Tile
 * @brief A class representing a tile in a tile-based system.
 *
 * The `Tile` class is a derived class from `TileBase`, adding additional functionality for positioning and rendering
 * tiles in a grid-based environment.
 */
class Tile : public TileBase
{
  public:
    /**
     * @brief Constructs a new Tile object.
     *
     * This constructor initializes a tile with specific attributes, such as its name, tag, texture, position in the
     * grid, and visual properties like scale and color.
     *
     * @param name The name of the tile.
     * @param tag The tag of the tile.
     * @param id The unique identifier of the tile.
     * @param texture The texture to use for the tile.
     * @param texture_rect The rectangle on the texture to use for the tile.
     * @param grid_size The size of the grid.
     * @param grid_position The position of the tile in the grid.
     * @param scale The scale factor for the tile.
     * @param color The color of the tile (defaults to white).
     */
    Tile(const std::string name, const std::string &tag, const uint64_t &id, sf::Texture &texture,
         const sf::IntRect &texture_rect, const unsigned int &grid_size, const sf::Vector2u &grid_position,
         const unsigned int &scale, const sf::Color &color = sf::Color::White);

    /**
     * @brief Destroys the Tile object.
     *
     * The destructor handles any necessary cleanup for the `Tile` object.
     */
    ~Tile();

    /**
     * @brief Updates the Tile object.
     *
     * This method is intended for updating the state of the tile, such as animations or interactions.
     * It is currently empty but can be extended in the future for more functionality.
     */
    void update();

    /**
     * @brief Renders the tile on the target.
     *
     * This method draws the tile's sprite to the provided render target.
     *
     * @param target The render target to draw the tile on.
     */
    void render(sf::RenderTarget &target);
};
