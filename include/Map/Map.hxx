#pragma once

#include "Engine/Configuration.hxx"
#include "Map/Biome.hxx"
#include "Map/Chunk.hxx"
#include "Map/PerlinNoise.hxx"
#include "Tiles/Tile.hxx"
#include "Tools/JSON.hxx"

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

struct Metadata
{
    struct DataPacks
    {
        JSONArray enabled;
        JSONArray disabled;
    };

    std::string dataVersion;
    std::string gameVersion;
    long long creationTime;
    DataPacks dataPacks;
    long long dayTime;
    std::string difficulty;
    std::string generatorName;
    long long lastPlayed;
    std::string name;
    long long seed;
    long long spawnX;
    long long spawnY;
    long long timePlayed;
};

inline JSONObject &operator<<(JSONObject &obj, Metadata &metadata)
{
    obj = JSONObject{{"dataVersion", metadata.dataVersion},
                     {"gameVersion", metadata.gameVersion},
                     {"dataPacks",
                      JSONObject{
                          {"enabled", metadata.dataPacks.enabled},
                          {"disabled", metadata.dataPacks.disabled},
                      }},
                     {"dayTime", metadata.dayTime},
                     {"name", metadata.name},
                     {"difficulty", metadata.difficulty},
                     {"seed", metadata.seed},
                     {"generatorName", metadata.generatorName},
                     {"creationTime", metadata.creationTime},
                     {"lastPlayed", metadata.lastPlayed},
                     {"spawnX", metadata.spawnX},
                     {"spawnY", metadata.spawnY},
                     {"timePlayed", metadata.timePlayed}};

    return obj;
}

inline JSONObject &operator>>(JSONObject &obj, Metadata &metadata)
{
    metadata.dataVersion = obj.at("dataVersion").get<std::string>();
    metadata.gameVersion = obj.at("gameVersion").get<std::string>();
    metadata.creationTime = obj.at("creationTime").get<long long>();
    metadata.dataPacks.enabled = obj.at("dataPacks").get<JSONObject>().at("enabled").get<JSONArray>();
    metadata.dataPacks.disabled = obj.at("dataPacks").get<JSONObject>().at("disabled").get<JSONArray>();
    metadata.dayTime = obj.at("dayTime").get<long long>();
    metadata.difficulty = obj.at("difficulty").get<std::string>();
    metadata.generatorName = obj.at("generatorName").get<std::string>();
    metadata.lastPlayed = obj.at("lastPlayed").get<long long>();
    metadata.name = obj.at("name").get<std::string>();
    metadata.seed = obj.at("seed").get<long long>();
    metadata.spawnX = obj.at("spawnX").get<long long>();
    metadata.spawnY = obj.at("spawnY").get<long long>();

    return obj;
}

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

    void putTile(Tile &tile, const unsigned int &grid_x, const unsigned int &grid_y, const unsigned int &grid_z);

    std::optional<Tile> getTile(const int &grid_x, const int &grid_y, const int &grid_z);

    void save(const std::string &name);

    void save();

    void load(const std::string &name);

    const sf::Vector2f getSpawnPoint() const;
};