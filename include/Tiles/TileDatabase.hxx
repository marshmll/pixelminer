/**
 * @file TileDatabase.hxx
 * @brief Header file for the TileDatabase class.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "Tiles/TileData.hxx"

/**
 * @class TileDatabase
 * @brief Manages a database of tile data.
 *
 * This class is responsible for storing and retrieving tile data, including tile tags, names, and texture rectangles.
 * It provides methods to insert new tiles and retrieve them by tag or ID.
 */
class TileDatabase
{
  private:
    std::hash<std::string> hash_function; ///< Hash function for generating tile IDs.

    std::unordered_map<std::string, TileData> db; ///< Database of tile data, indexed by tile tags.

  public:
    /**
     * @brief Constructs a TileDatabase object.
     *
     * Initializes the database with a default "unknown" tile.
     */
    TileDatabase();

    /**
     * @brief Destructor for the TileDatabase object.
     */
    ~TileDatabase();

    /**
     * @brief Inserts a new tile into the database.
     *
     * @param tag The unique tag for the tile.
     * @param name The name of the tile.
     * @param rect_index_x The x-coordinate index of the tile's texture rectangle.
     * @param rect_index_y The y-coordinate index of the tile's texture rectangle.
     * @param size_in_pixels The size of the tile in pixels.
     */
    void insert(const std::string &tag, const std::string &name, const int &rect_index_x, const int &rect_index_y,
                const int &size_in_pixels);

    /**
     * @brief Retrieves tile data by its tag.
     *
     * @param tag The tag of the tile to retrieve.
     * @return The TileData object corresponding to the tag. If the tag is not found, returns the "unknown" tile data.
     */
    const TileData getByTag(const std::string &tag);

    /**
     * @brief Retrieves tile data by its ID.
     *
     * @param id The ID of the tile to retrieve.
     * @return The TileData object corresponding to the ID. If the ID is not found, returns the "unknown" tile data.
     */
    const TileData getById(const uint64_t &id);
};
