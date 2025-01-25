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

class TerrainGenerator
{
  private:
    Logger logger;

    Metadata &metadata;
    ChunkMatrix &chunks;

    long int seed;

    sf::Texture &texturePack;
    std::map<std::uint32_t, TileData> &tileData;

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
    std::array<std::array<BiomeData, MAX_WORLD_GRID_SIZE.y>, MAX_WORLD_GRID_SIZE.x> biomeMap;

    void initPerlinWaves();

    void initNoiseMaps();

    void initBiomes();

    void putTile(Tile tile, const int &grid_x, const int &grid_y, const int &grid_z);

    Tile *getTile(const int &grid_x, const int &grid_y, const int &grid_z);

  public:
    TerrainGenerator(Metadata &metadata, ChunkMatrix &chunks, const long int seed, sf::Texture &texture_pack,
                     std::map<std::uint32_t, TileData> &tile_data, const unsigned int &grid_size, const float &scale);

    ~TerrainGenerator();

    void generateRegion(const sf::Vector2i &region_index);
};