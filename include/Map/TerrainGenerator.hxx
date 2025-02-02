#pragma once

#include "Map/Biome.hxx"
#include "Map/Chunk.hxx"
#include "Map/Metadata.hxx"
#include "Map/PerlinNoise.hxx"
#include "Tiles/TileData.hxx"
#include "Tools/Logger.hxx"

static constexpr sf::Vector2u MAX_REGIONS = {16, 16};
static constexpr sf::Vector2u MAX_CHUNKS = {MAX_REGIONS.x * REGION_SIZE_IN_CHUNKS.x,
                                            MAX_REGIONS.y *REGION_SIZE_IN_CHUNKS.y};

static constexpr sf::Vector2u MAX_WORLD_GRID_SIZE = {
    MAX_CHUNKS.x * CHUNK_SIZE_IN_TILES.x,
    MAX_CHUNKS.y *CHUNK_SIZE_IN_TILES.y,
};

using ChunkMatrix = std::array<std::array<std::unique_ptr<Chunk>, MAX_CHUNKS.x>, MAX_CHUNKS.y>;
using BiomeMap = std::array<std::array<BiomePreset, MAX_WORLD_GRID_SIZE.y>, MAX_WORLD_GRID_SIZE.x>;

class TerrainGenerator
{
  private:
    Logger logger;

    std::string &msg;

    Metadata &metadata;
    ChunkMatrix &chunks;

    long int seed;

    sf::Texture &texturePack;
    std::unordered_map<std::string, TileData> &tileDB;

    unsigned int gridSize;
    float scale;

    Random rng;

    PerlinNoise perlinNoise;

    std::vector<Wave> heightWaves;
    std::vector<Wave> moistureWaves;
    std::vector<Wave> heatWaves;

    NoiseMap heightMap;
    NoiseMap moistureMap;
    NoiseMap heatMap;

    std::vector<Biome> biomes;
    BiomeMap biomeMap;
    std::vector<std::vector<float>> randomGrid; // Precomputed random values for the entire world

    void initPerlinWaves();

    void initNoiseMaps();

    void initBiomes();

    void initRandomGrid();

    void putTile(Tile tile, const int &grid_x, const int &grid_y, const int &grid_z);

    Tile *getTile(const int &grid_x, const int &grid_y, const int &grid_z);

  public:
    TerrainGenerator(std::string &msg, Metadata &metadata, ChunkMatrix &chunks, const long int seed,
                     sf::Texture &texture_pack, std::unordered_map<std::string, TileData> &tile_db,
                     const unsigned int &grid_size, const float &scale);

    ~TerrainGenerator();

    void generateRegion(const sf::Vector2i &region_index);

    const BiomePreset &getBiomeData(const sf::Vector2u &grid_pos) const;

    const float &getHeightAt(const sf::Vector2u &grid_pos) const;

    const float &getMoistureAt(const sf::Vector2u &grid_pos) const;

    const float &getHeatAt(const sf::Vector2u &grid_pos) const;
};