/**
 * @file Chunk.hxx
 * @brief Declares the Chunk class to manage collections of tiles in a map.
 */

#pragma once

#include "Tiles/Tile.hxx"

/**
 * @enum ChunkFlags
 * @brief Enum representing different flags that can be applied to a chunk.
 *
 * This enum defines flags that specify various properties or states of a chunk.
 */
enum ChunkFlags : uint8_t
{
    None = 0x0,       ///< No flags set for the chunk.
    Modified = 0x1,   ///< Indicates that the chunk is different from default generation.
    KeepLoaded = 0x2, ///< Indicates that the chunk should not be unloaded from memory.
};

/**
 * @brief The size of a region in chunks (8x8 chunks).
 */
static constexpr sf::Vector2u REGION_SIZE_IN_CHUNKS = {8, 8};

/**
 * @brief The size of a chunk in tiles (16x16x5 tiles).
 */
static constexpr sf::Vector3<unsigned int> CHUNK_SIZE_IN_TILES = {16, 16, 5};

/**
 * @typedef TileMatrix
 * @brief A 3D array representing a chunk's grid of tiles.
 *
 * This type alias defines a 3D array of `Tile` objects, with dimensions defined by `CHUNK_SIZE_IN_TILES`.
 * Each element in the array is a `std::unique_ptr` to a `Tile` object.
 */
using TileMatrix =
    std::array<std::array<std::array<std::unique_ptr<Tile>, CHUNK_SIZE_IN_TILES.z>, CHUNK_SIZE_IN_TILES.y>,
               CHUNK_SIZE_IN_TILES.x>;

/**
 * @class Chunk
 * @brief Represents a chunk of tiles within a larger map.
 *
 * A chunk is a portion of the map consisting of a grid of tiles. The chunk is characterized by its size,
 * position, and state flags. It can be rendered and updated as part of the map.
 */
class Chunk
{
  public:
    sf::RectangleShape chunkBorders;      ///< Visual border of the chunk for debugging.
    sf::Vector2<unsigned int> chunkIndex; ///< The index or position of the chunk in the grid.

    unsigned int gridSize; ///< Size of the grid (in pixels) used for each tile in the chunk.
    float scale;           ///< The scaling factor applied to the chunk's size.

    TileMatrix tiles; ///< The 3D grid of tiles that make up the chunk.

    uint8_t flags; ///< Flags that specify properties or states of the chunk (from ChunkFlags enum).

    /**
     * @brief Constructs a Chunk object with the specified parameters.
     *
     * @param chunk_index The index (position) of the chunk within the larger grid.
     * @param grid_size The size of each tile in the chunk, in pixels.
     * @param scale The scaling factor applied to the chunk's size.
     * @param flags Optional flags to configure the chunk (default is `ChunkFlags::None`).
     */
    Chunk(const sf::Vector2u chunk_index, const unsigned int &grid_size, const float &scale,
          uint8_t flags = ChunkFlags::None);

    /**
     * @brief Destructor for the Chunk class.
     */
    ~Chunk();

    /**
     * @brief Updates the chunk's state.
     *
     * This method is called each frame to update the chunk. It currently doesn't do anything but is
     * present for future updates, such as managing chunk-based animations or states.
     *
     * @param dt The time elapsed since the last update.
     */
    void update(const float &dt);

    /**
     * @brief Renders the chunk to the specified render target.
     *
     * This function draws all the tiles within the chunk to the provided render target. Optionally,
     * a border can be drawn for debugging purposes.
     *
     * @param target The render target to draw the chunk to (e.g., a window or a texture).
     * @param debug If true, the chunk's border is drawn for debugging.
     */
    void render(sf::RenderTarget &target, const bool &debug = false);
};
