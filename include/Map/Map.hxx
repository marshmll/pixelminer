#pragma once

#include "Map/Biome.hxx"
#include "Map/Chunk.hxx"
#include "Map/PerlinNoise.hxx"
#include "Tiles/Tile.hxx"

static constexpr sf::Vector2u MAX_REGIONS = {4, 4}; // TEMP
static constexpr sf::Vector2u MAX_CHUNKS = {MAX_REGIONS.x * REGION_SIZE_IN_CHUNKS.x,
                                            MAX_REGIONS.y *REGION_SIZE_IN_CHUNKS.y};

static constexpr sf::Vector2u MAX_WORLD_GRID_SIZE = {
    MAX_CHUNKS.x * CHUNK_SIZE_IN_TILES.x,
    MAX_CHUNKS.y *CHUNK_SIZE_IN_TILES.y,
};

struct BiomeData
{
    BiomeType type;
    sf::Color color;
};

class Map
{
  private:
    std::filesystem::path path;

    std::map<std::uint32_t, TileData> &tileData;
    sf::Texture &texturePack;

    unsigned int gridSize;
    float scale;

    std::array<std::array<std::unique_ptr<Chunk>, MAX_CHUNKS.x>, MAX_CHUNKS.y> chunks;
    std::array<std::array<BiomeData, MAX_WORLD_GRID_SIZE.y>, MAX_WORLD_GRID_SIZE.x> biomeMap;

    sf::Vector2f spawnPoint;

    long long int seed;
    std::unique_ptr<PerlinNoise> noise;

    std::vector<Wave> height_waves;
    std::vector<Wave> moisture_waves;
    std::vector<Wave> heat_waves;

    NoiseMap height_map;
    NoiseMap moisture_map;
    NoiseMap heat_map;

    std::vector<Biome> biomes;

    void resize();

    void initPerlinWaves();

    void initNoiseMaps();

    void initBiomes();

    void randomizeSpawnPoint();

  public:
    Map(std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack, const unsigned int &grid_size,
        const float &scale, const long int &seed);

    Map(const std::filesystem::path path, std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack,
        const unsigned int &grid_size, const float &scale);

    ~Map();

    const sf::Vector2f getRealDimensions() const;

    void generate();

    void update(const float &dt);

    void render(sf::RenderTarget &target, const bool &debug = false);

    void render(sf::RenderTarget &target, const sf::Vector2i &entity_pos_grid, const bool &debug);

    void putTile(Tile &tile, const unsigned int &grid_x, const unsigned int &grid_y, const unsigned int &grid_z);

    std::optional<Tile> getTile(const int &grid_x, const int &grid_y, const int &grid_z);

    void saveToFile(std::filesystem::path path);

    void save();

    void loadFromFile(std::filesystem::path path);

    const sf::Vector2f &getSpawnPoint() const;
};