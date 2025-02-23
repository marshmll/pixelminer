/**
 * @file Map.hxx
 * @brief Declares the Map class to manage worlds and tilemaps.
 */

#pragma once

#include "Map/TerrainGenerator.hxx"
#include "Tiles/Tile.hxx"
#include "Tiles/TileDatabase.hxx"
#include "Tools/JSON.hxx"
#include "Tools/LinearCongruentialGenerator.hxx"
#include "Tools/Logger.hxx"

/**
 * @class Map
 * @brief Class for managing the world map, including terrain generation, chunk loading, and saving/loading regions.
 *
 * This class handles the map's grid, terrain generation, chunk management, and saving/loading of regions.
 * It also takes care of multithreading tasks such as loading/unloading regions based on the player's position.
 */
class Map
{
  private:
    std::mutex mutex; ///< Mutex to ensure thread safety when accessing shared resources.

    Logger logger; ///< Logger for logging information and errors.

    sf::Clock clock; ///< Clock to track time for map-related activities.

    std::atomic_bool ready; ///< Flag indicating whether the map is ready for interaction.
    std::string msg;        ///< Message for user feedback about the map loading process.

    std::string folderName; ///< Folder name where the map's data is stored.
    Metadata metadata;      ///< Metadata related to the map (e.g., name, seed, version).

    std::unique_ptr<TerrainGenerator> terrainGenerator; ///< Unique pointer to the terrain generator.

    TileDatabase &tileDb;     ///< Reference to the tile database.
    sf::Texture &texturePack; ///< Reference to the texture pack used for tiles.

    unsigned int gridSize; ///< Size of the grid (e.g., how many tiles per chunk).
    float scale;           ///< Scaling factor for rendering the map.

    ChunkMatrix chunks;                                           ///< 2D array of chunks in the map.
    std::atomic_bool loadedRegions[MAX_REGIONS.x][MAX_REGIONS.y]; ///< Array to track loaded regions.

    Random rng; ///< Random number generator for procedural generation.

    /**
     * @brief Initializes the loading screen process.
     */
    void initLoadingScreen();

    /**
     * @brief Initializes the array to track the status of loaded regions.
     */
    void initRegionStatusArray();

    /**
     * @brief Initializes the metadata for the map.
     * @param name Name of the map.
     * @param seed Seed used for random generation.
     */
    void initMetadata(const std::string &name, const long int &seed);

    /**
     * @brief Initializes the terrain generator.
     * @param seed Seed for terrain generation.
     */
    void initTerrainGenerator(const long int &seed);

    /**
     * @brief Sets the readiness status of the map.
     * @param ready The readiness state to set.
     */
    void setReady(const bool ready);

  public:
    /**
     * @brief Constructor that initializes a new map with a specific name and seed.
     * @param name Name of the map.
     * @param seed Seed for world generation.
     * @param tile_db Reference to the tile database.
     * @param texture_pack Reference to the texture pack used.
     * @param grid_size Size of the grid (number of tiles per chunk).
     * @param scale Scaling factor for rendering the map.
     */
    Map(const std::string &name, const long int &seed, TileDatabase &tile_db, sf::Texture &texture_pack,
        const unsigned int &grid_size, const float &scale);

    /**
     * @brief Constructor that initializes an empty map.
     * @param tile_db Reference to the tile database.
     * @param texture_pack Reference to the texture pack used.
     * @param grid_size Size of the grid (number of tiles per chunk).
     * @param scale Scaling factor for rendering the map.
     */
    Map(TileDatabase &tile_db, sf::Texture &texture_pack, const unsigned int &grid_size, const float &scale);

    /**
     * @brief Destructor for cleaning up the map.
     */
    ~Map();

    /**
     * @brief Updates the map based on the player's position and time delta.
     * @param dt Time delta for updating the map.
     * @param player_pos_grid Player's position in the grid.
     */
    void update(const float &dt, const sf::Vector2i &player_pos_grid);

    /**
     * @brief Renders the map to a render target.
     * @param target The render target to render to.
     * @param debug Flag to indicate if debug information should be rendered.
     */
    void render(sf::RenderTarget &target, const bool &debug = false);

    /**
     * @brief Renders the map based on an entity's position in the grid.
     * @param target The render target to render to.
     * @param entity_pos_grid The entity's position in the grid.
     * @param debug Flag to indicate if debug information should be rendered.
     */
    void render(sf::RenderTarget &target, const sf::Vector2i &entity_pos_grid, const bool &debug);

    /**
     * @brief Saves the map to a file with a specific name.
     * @param name The name of the map to save.
     */
    void save(const std::string &name);

    /**
     * @brief Saves the current map to a file.
     */
    void save();

    /**
     * @brief Saves the specified region of the map to a file.
     * @param region_index The index of the region to save.
     */
    void saveRegion(const sf::Vector2i &region_index);

    /**
     * @brief Loads a map from a file with a specific name.
     * @param name The name of the map to load.
     */
    void load(const std::string &name);

    /**
     * @brief Loads the specified region of the map from a file.
     * @param region_index The index of the region to load.
     */
    void loadRegion(const sf::Vector2i &region_index);

    /**
     * @brief Unloads the specified region of the map.
     * @param region_index The index of the region to unload.
     */
    void unloadRegion(const sf::Vector2i &region_index);

    /**
     * @brief Places a tile in the world at the specified coordinates.
     * @param tile The tile to place.
     * @param grid_x The x-coordinate in the grid.
     * @param grid_y The y-coordinate in the grid.
     * @param grid_z The z-coordinate in the grid.
     */
    void putTile(Tile tile, const int &grid_x, const int &grid_y, const int &grid_z);

    /**
     * @brief Retrieves a tile at the specified coordinates.
     * @param grid_x The x-coordinate in the grid.
     * @param grid_y The y-coordinate in the grid.
     * @param grid_z The z-coordinate in the grid.
     * @return A pointer to the tile at the specified coordinates.
     */
    Tile *getTile(const int &grid_x, const int &grid_y, const int &grid_z);

    /**
     * @brief Retrieves the top-most tile at the specified coordinates.
     * @param grid_x The x-coordinate in the grid.
     * @param grid_y The y-coordinate in the grid.
     * @return A pointer to the tile at the specified coordinates.
     */
    Tile *getTile(const int &grid_x, const int &grid_y);

    /**
     * @brief Removes a tile at the specified coordinates.
     * @param grid_x The x-coordinate in the grid.
     * @param grid_y The y-coordinate in the grid.
     * @param grid_z The z-coordinate in the grid.
     * @return If the tile was removed.
     */
    const bool removeTile(const int &grid_x, const int &grid_y, const int &grid_z);

    /**
     * @brief Removes the top-most tile at the specified coordinates.
     * @param grid_x The x-coordinate in the grid.
     * @param grid_y The y-coordinate in the grid.
     * @return If the tile was removed.
     */
    const bool removeTile(const int &grid_x, const int &grid_y);

    /**
     * @brief Retrieves the spawn point of the map.
     * @return A vector representing the spawn point of the map.
     */
    const sf::Vector2f getSpawnPoint() const;

    /**
     * @brief Retrieves the real dimensions of the map.
     * @return A vector representing the real dimensions of the map.
     */
    const sf::Vector2f getRealDimensions() const;

    /**
     * @brief Retrieves the biome at the specified grid coordinates.
     * @param grid_coords The grid coordinates to check.
     * @return The biome preset at the specified grid coordinates.
     */
    const BiomePreset getBiomeAt(const sf::Vector2i &grid_coords) const;

    /**
     * @brief Retrieves the height at the specified grid coordinates.
     * @param grid_pos The grid coordinates to check.
     * @return The height value at the specified grid coordinates.
     */
    const float getHeightAt(const sf::Vector2i &grid_pos) const;

    /**
     * @brief Retrieves the moisture at the specified grid coordinates.
     * @param grid_pos The grid coordinates to check.
     * @return The moisture value at the specified grid coordinates.
     */
    const float getMoistureAt(const sf::Vector2i &grid_pos) const;

    /**
     * @brief Retrieves the heat at the specified grid coordinates.
     * @param grid_pos The grid coordinates to check.
     * @return The heat value at the specified grid coordinates.
     */
    const float getHeatAt(const sf::Vector2i &grid_pos) const;

    /**
     * @brief Checks if the map is ready for interaction.
     * @return True if the map is ready, otherwise false.
     */
    const bool isReady() const;

    /**
     * @brief Retrieves the current loading message.
     * @return The current message about the loading status.
     */
    const std::string &getMessage() const;

    /**
     * @brief Retrieves the folder name where the map is saved.
     * @return The folder name.
     */
    const std::string &getFolderName() const;

    /**
     * @brief Checks if a specific region is loaded.
     * @param region_index The index of the region to check.
     * @return True if the region is loaded, false if not, `std::nullopt`
     * if region is out of bounds.
     */
    const std::optional<bool> isRegionLoaded(const sf::Vector2i &region_index);
};
