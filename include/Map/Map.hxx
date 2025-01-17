#pragma once

#include "Engine/Configuration.hxx"
#include "Map/Biome.hxx"
#include "Map/Chunk.hxx"
#include "Map/Metadata.hxx"
#include "Map/PerlinNoise.hxx"
#include "Tiles/Tile.hxx"
#include "Tools/JSON.hxx"
#include "Tools/LinearCongruentialGenerator.hxx"

static constexpr sf::Vector2u MAX_REGIONS = {4, 4}; // TEMP
static constexpr sf::Vector2u MAX_CHUNKS = {MAX_REGIONS.x * REGION_SIZE_IN_CHUNKS.x,
                                            MAX_REGIONS.y *REGION_SIZE_IN_CHUNKS.y};

static constexpr sf::Vector2u MAX_WORLD_GRID_SIZE = {
    MAX_CHUNKS.x * CHUNK_SIZE_IN_TILES.x,
    MAX_CHUNKS.y *CHUNK_SIZE_IN_TILES.y,
};

class Map
{
  private:
    Metadata metadata;

    std::map<std::uint32_t, TileData> &tileData;
    sf::Texture &texturePack;

    unsigned int gridSize;
    float scale;

    std::array<std::array<std::unique_ptr<Chunk>, MAX_CHUNKS.x>, MAX_CHUNKS.y> chunks;
    std::array<std::array<BiomeData, MAX_WORLD_GRID_SIZE.y>, MAX_WORLD_GRID_SIZE.x> biomeMap;

    std::unique_ptr<PerlinNoise> noise;

    std::vector<Wave> height_waves;
    std::vector<Wave> moisture_waves;
    std::vector<Wave> heat_waves;

    NoiseMap height_map;
    NoiseMap moisture_map;
    NoiseMap heat_map;

    std::vector<Biome> biomes;

    Random rng;

    void initPerlinWaves();

    void initNoiseMaps();

    void initBiomes();

    void randomizeSpawnPoint();

  public:
    Map(std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack, const unsigned int &grid_size,
        const float &scale, const long int &seed);

    Map(const std::string &name, std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack,
        const unsigned int &grid_size, const float &scale);

    ~Map();

    const sf::Vector2f getRealDimensions() const;

    void generate();

    void update(const float &dt);

    void render(sf::RenderTarget &target, const bool &debug = false);

    void render(sf::RenderTarget &target, const sf::Vector2i &entity_pos_grid, const bool &debug);

    void putTile(Tile tile, const int &grid_x, const int &grid_y, const int &grid_z);

    Tile *getTile(const int &grid_x, const int &grid_y, const int &grid_z);

    void save(const std::string &name);

    void save();

    void load(const std::string &name);

    const sf::Vector2f getSpawnPoint() const;
};