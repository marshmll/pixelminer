#pragma once

#include "Engine/Configuration.hxx"
#include "Map/Metadata.hxx"
#include "Map/TerrainGenerator.hxx"
#include "Tiles/Tile.hxx"
#include "Tools/JSON.hxx"
#include "Tools/LinearCongruentialGenerator.hxx"

class Map
{
  private:
    Metadata metadata;

    std::unique_ptr<TerrainGenerator> terrainGenerator;

    std::map<std::uint32_t, TileData> &tileData;
    sf::Texture &texturePack;

    unsigned int gridSize;
    float scale;

    ChunkMatrix chunks;

    Random rng;

    void initMetadata(const std::string &name, const long int &seed);

    void initTerrainGenerator(const long int &seed);

  public:
    /**
     * @brief Generates a new world with the given name and seed.
     */
    Map(const std::string &name, const long int &seed, std::map<std::uint32_t, TileData> &tile_data,
        sf::Texture &texture_pack, const unsigned int &grid_size, const float &scale);

    /**
     * @brief Generates a empty world.
     */
    Map(std::map<std::uint32_t, TileData> &tile_data, sf::Texture &texture_pack, const unsigned int &grid_size,
        const float &scale);

    ~Map();

    void update(const float &dt);

    void render(sf::RenderTarget &target, const bool &debug = false);

    void render(sf::RenderTarget &target, const sf::Vector2i &entity_pos_grid, const bool &debug);

    void save(const std::string &name);

    void save();

    void load(const std::string &name);

    void loadRegion(const sf::Vector2u &region_index);

    void putTile(Tile tile, const int &grid_x, const int &grid_y, const int &grid_z);

    Tile *getTile(const int &grid_x, const int &grid_y, const int &grid_z);

    const sf::Vector2f getSpawnPoint() const;

    const sf::Vector2f getRealDimensions() const;
};