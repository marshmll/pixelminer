/**
 * @file TileData.hxx
 * @brief Declares the TileData struct to pack all tile relevant data in a single structure.
 */

#pragma once

#include "stdafx.hxx"

/**
 * @struct TileData
 * @brief A structure that holds data for a tile.
 *
 * This structure is used to store information about a tile, such as its unique identifier, name,
 * texture rectangle, and size. It is typically used for managing tile properties in tile-based games.
 */
struct TileData
{
    /**
     * @brief The unique identifier of the tile.
     *
     * This is a string used to uniquely identify each tile.
     */
    std::string id;

    /**
     * @brief The name of the tile.
     *
     * This is a string that provides a name or label for the tile.
     */
    std::string name;

    /**
     * @brief The rectangle that defines the texture's position and size.
     *
     * The `sf::IntRect` defines the portion of the texture that is used for the tile. It contains
     * the x and y position of the texture, along with its width and height.
     */
    sf::IntRect rect;

    /**
     * @brief The size of the tile.
     *
     * This is an integer that represents the size of the tile (e.g., width/height or tile size in pixels).
     */
    int size;
};
