#pragma once

#include "Map/Biome.hxx"
#include "Map/Chunk.hxx"
#include "Map/PerlinNoise.hxx"
#include "Tiles/Tile.hxx"

class Map
{
  private:
    sf::Vector2u amountOfChunks;
    sf::Vector3<unsigned int> dimensions;

    std::map<std::uint32_t, TileData> &tileData;
    sf::Texture &texturePack;

    unsigned int gridSize;
    float scale;

    std::vector<std::vector<std::unique_ptr<Chunk>>> chunks;

    sf::Vector2f spawnPoint;

    PerlinNoise noise;

    std::vector<Wave> height_waves;
    std::vector<Wave> moisture_waves;
    std::vector<Wave> heat_waves;

    NoiseMap height_map;
    NoiseMap moisture_map;
    NoiseMap heat_map;

    std::vector<Biome> biomes;

    void clear();

    void resize(const sf::Vector2<unsigned int> &amount_of_chunks);

    void initPerlinWaves();

    void initNoiseMaps();

    void initBiomes();

    void randomizeSpawnPoint();

  public:
    Map(const sf::Vector2<unsigned int> &amount_of_chunks, std::map<std::uint32_t, TileData> &tile_data,
        sf::Texture &texture_pack, const unsigned int &grid_size, const float &scale);

    Map(const std::filesystem::path path, std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack,
        const unsigned int &grid_size, const float &scale);

    ~Map();

    const sf::Vector3<unsigned int> &getDimensions() const;

    void generate();

    void update(const float &dt);

    void render(sf::RenderTarget &target);

    void putTile(TileBase tile, const unsigned int &x, const unsigned int &y, const unsigned int &z);

    void saveToFile(std::filesystem::path path);

    void loadFromFile(std::filesystem::path path);

    const sf::Vector2f &getSpawnPoint() const;
};