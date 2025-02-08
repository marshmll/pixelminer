/**
 * @file TerrainGenerator.hxx
 * @brief Declares the TerrainGenerator class to manage procedural world generation based on seeds.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "Map/Biome.hxx"
#include "Map/Chunk.hxx"
#include "Map/Metadata.hxx"
#include "Tiles/TileData.hxx"
#include "Tools/Logger.hxx"
#include "Tools/PerlinNoise.hxx"

/// Maximum number of regions in the world grid (16x16).
static constexpr sf::Vector2u MAX_REGIONS = {16, 16};

/// Maximum number of chunks in the world grid based on regions.
static constexpr sf::Vector2u MAX_CHUNKS = {MAX_REGIONS.x * REGION_SIZE_IN_CHUNKS.x,
                                            MAX_REGIONS.y *REGION_SIZE_IN_CHUNKS.y};

/// Maximum world grid size in tiles (based on chunks and region sizes).
static constexpr sf::Vector2u MAX_WORLD_GRID_SIZE = {
    MAX_CHUNKS.x * CHUNK_SIZE_IN_TILES.x,
    MAX_CHUNKS.y *CHUNK_SIZE_IN_TILES.y,
};

/// Defines a matrix of chunks for the world.
using ChunkMatrix = std::array<std::array<std::unique_ptr<Chunk>, MAX_CHUNKS.x>, MAX_CHUNKS.y>;

/// Defines the biome map for the world grid.
using BiomeMap = std::array<std::array<BiomePreset, MAX_WORLD_GRID_SIZE.y>, MAX_WORLD_GRID_SIZE.x>;

/// Class responsible for terrain generation in the world grid.
class TerrainGenerator
{
  private:
    Logger logger;                                     ///< Logger used for debugging and messages.
    std::string &msg;                                  ///< String to store messages for reporting progress.
    Metadata &metadata;                                ///< Metadata related to world generation.
    ChunkMatrix &chunks;                               ///< Matrix storing chunks of the world.
    long int seed;                                     ///< Seed for random number generation and Perlin noise.
    sf::Texture &texturePack;                          ///< Texture pack for tile rendering.
    std::unordered_map<std::string, TileData> &tileDB; ///< Tile database mapping tile IDs to tile data.
    unsigned int gridSize;                             ///< Size of the world grid.
    float scale;                                       ///< Scale for terrain generation.
    Random rng;                                        ///< Random number generator.
    PerlinNoise perlinNoise;                           ///< Perlin noise generator for terrain features.

    std::vector<Wave> heightWaves;              ///< Waves for heightmap generation.
    std::vector<Wave> moistureWaves;            ///< Waves for moisture map generation.
    std::vector<Wave> heatWaves;                ///< Waves for heatmap generation.
    NoiseMap heightMap;                         ///< Height map of the world.
    NoiseMap moistureMap;                       ///< Moisture map of the world.
    NoiseMap heatMap;                           ///< Heat map of the world.
    std::vector<Biome> biomes;                  ///< List of biomes available for the world.
    BiomeMap biomeMap;                          ///< Map of biomes for the world grid.
    std::vector<std::vector<float>> randomGrid; ///< Precomputed random values for the entire world.

    /// Initializes Perlin noise waves for height, moisture, and heat.
    void initPerlinWaves();

    /// Initializes noise maps (height, moisture, heat) for the world grid.
    void initNoiseMaps();

    /// Initializes the list of biomes and precomputes biome data.
    void initBiomes();

    /// Initializes a grid of random values used for world generation.
    void initRandomGrid();

    /// Places a tile in the world grid at the specified position.
    void putTile(Tile tile, const int &grid_x, const int &grid_y, const int &grid_z);

    /// Retrieves the tile at the specified grid position.
    Tile *getTile(const int &grid_x, const int &grid_y, const int &grid_z);

  public:
    /**
     * @brief Constructor for TerrainGenerator.
     *
     * Initializes the terrain generation system, including Perlin noise, biome map, and noise maps.
     *
     * @param msg Reference to a string for storing messages.
     * @param metadata Metadata related to world generation.
     * @param chunks Chunk matrix to store world chunks.
     * @param seed Seed for random number generation and Perlin noise.
     * @param texture_pack Reference to the texture pack for tile rendering.
     * @param tile_db Reference to the tile database.
     * @param grid_size Grid size for world generation.
     * @param scale Scale factor for terrain generation.
     */
    TerrainGenerator(std::string &msg, Metadata &metadata, ChunkMatrix &chunks, long int seed,
                     sf::Texture &texture_pack, std::unordered_map<std::string, TileData> &tile_db,
                     const unsigned int &grid_size, const float &scale);

    /// Destructor for TerrainGenerator.
    ~TerrainGenerator();

    /**
     * @brief Generates terrain for a specific region in the world.
     *
     * @param region_index The index of the region to generate (x, y).
     */
    void generateRegion(const sf::Vector2i &region_index);

    /**
     * @brief Retrieves the biome data for a specific grid position.
     *
     * @param grid_pos The position (x, y) on the world grid.
     * @return const BiomePreset& The biome data at the specified grid position.
     */
    const BiomePreset &getBiomeData(const sf::Vector2u &grid_pos) const;

    /**
     * @brief Retrieves the height value at a specific grid position.
     *
     * @param grid_pos The position (x, y) on the world grid.
     * @return const float& The height value at the specified grid position.
     */
    const float &getHeightAt(const sf::Vector2u &grid_pos) const;

    /**
     * @brief Retrieves the moisture value at a specific grid position.
     *
     * @param grid_pos The position (x, y) on the world grid.
     * @return const float& The moisture value at the specified grid position.
     */
    const float &getMoistureAt(const sf::Vector2u &grid_pos) const;

    /**
     * @brief Retrieves the heat value at a specific grid position.
     *
     * @param grid_pos The position (x, y) on the world grid.
     * @return const float& The heat value at the specified grid position.
     */
    const float &getHeatAt(const sf::Vector2u &grid_pos) const;
};
