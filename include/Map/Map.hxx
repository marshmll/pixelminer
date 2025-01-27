#pragma once

#include "Engine/Configuration.hxx"
#include "Map/TerrainGenerator.hxx"
#include "Tiles/Tile.hxx"
#include "Tools/JSON.hxx"
#include "Tools/LinearCongruentialGenerator.hxx"

class Map
{
  private:
    std::mutex mutex;

    std::atomic_bool ready; // Flag for loading status.
    std::string msg;        // User feedback

    Metadata metadata;

    std::unique_ptr<TerrainGenerator> terrainGenerator;

    std::map<std::string, TileData> &tileDB;
    sf::Texture &texturePack;

    unsigned int gridSize;
    float scale;

    ChunkMatrix chunks;
    std::atomic_bool loadedRegions[MAX_REGIONS.x][MAX_REGIONS.y];

    Random rng;

    void initLoadingScreen();

    void initRegionStatusArray();

    void initMetadata(const std::string &name, const long int &seed);

    void initTerrainGenerator(const long int &seed);

    void setReady(const bool ready);

  public:
    /**
     * @brief Generates a new world with the given name and seed.
     */
    Map(const std::string &name, const long int &seed, std::map<std::string, TileData> &tile_db,
        sf::Texture &texture_pack, const unsigned int &grid_size, const float &scale);

    /**
     * @brief Generates a empty world.
     */
    Map(std::map<std::string, TileData> &tile_db, sf::Texture &texture_pack, const unsigned int &grid_size,
        const float &scale);

    ~Map();

    void update(const float &dt, const sf::Vector2i &player_pos_grid);

    void render(sf::RenderTarget &target, const bool &debug = false);

    void render(sf::RenderTarget &target, const sf::Vector2i &entity_pos_grid, const bool &debug);

    void save(const std::string &name);

    void save();

    void saveRegion(const sf::Vector2i &region_index);

    void load(const std::string &name);

    void loadRegion(const sf::Vector2i &region_index);

    void unloadRegion(const sf::Vector2i &region_index);

    void putTile(Tile tile, const int &grid_x, const int &grid_y, const int &grid_z);

    Tile *getTile(const int &grid_x, const int &grid_y, const int &grid_z);

    const sf::Vector2f getSpawnPoint() const;

    const sf::Vector2f getRealDimensions() const;

    const bool isReady() const;

    const std::string &getMessage() const;

    const bool isRegionLoaded(const sf::Vector2i &region_index) const;
};