/**
 * @file EntitySpatialGridPartition.hxx
 * @brief Declares the EntitySpatialGridPartition class to manage entities in a spacial partition.
 */

#pragma once

#include "Engine/Configuration.hxx"
#include "Entities/Entity.hxx"
#include "Map/TerrainGenerator.hxx"

/**
 * @brief The size of each cell in the spatial grid partition (in tiles).
 */
constexpr unsigned int SPATIAL_GRID_PARTITION_CELL_SIZE_IN_TILES = 8;

/**
 * @brief The dimensions of the spatial grid partition, based on the world grid size.
 *
 * This is calculated using the maximum world grid size and the cell size, ensuring that the entire world
 * can be divided into smaller partitions of cells.
 */
static const sf::Vector2u SPATIAL_GRID_PARTITION_DIMENSIONS =
    sf::Vector2u(std::ceil(static_cast<float>(MAX_WORLD_GRID_SIZE.x) /
                           static_cast<float>(SPATIAL_GRID_PARTITION_CELL_SIZE_IN_TILES)),
                 std::ceil(static_cast<float>(MAX_WORLD_GRID_SIZE.y) /
                           static_cast<float>(SPATIAL_GRID_PARTITION_CELL_SIZE_IN_TILES)));

/**
 * @typedef Cell
 * @brief A type alias for a cell in the spatial grid partition.
 *
 * Each cell holds a vector of `std::shared_ptr<Entity>`, representing all the entities that occupy
 * the specific partitioned area in the spatial grid.
 */
using Cell = std::vector<std::shared_ptr<Entity>>;

/**
 * @typedef SpatialGridPartition
 * @brief A type alias for the entire spatial grid partition.
 *
 * This represents a 2D grid of cells, each holding a collection of entities.
 */
using SpatialGridPartition = std::vector<std::vector<Cell>>;

/**
 * @class EntitySpatialGridPartition
 * @brief Manages a spatial grid partition to organize and track entities in a grid-based system.
 *
 * This class manages a partition of the world grid, organizing entities into cells for efficient spatial queries.
 * It provides methods to put, remove, and move entities within the grid, as well as retrieve entities based
 * on their grid coordinates.
 */
class EntitySpatialGridPartition
{
  private:
    Logger logger;              ///< Logger for logging errors and warnings.
    SpatialGridPartition cells; ///< The 2D grid of cells where entities are stored.
    std::unordered_map<uint64_t, sf::Vector2i>
        entitySpacialGridLookUpTable; ///< A look-up table for entity IDs and their grid coordinates.
    float scale;                      ///< The scaling factor used for the grid.

  public:
    /**
     * @brief Constructs an EntitySpatialGridPartition object.
     *
     * Initializes the partition with the given scaling factor and resizes the grid to fit the world dimensions.
     *
     * @param scale The scaling factor to be applied to the spatial grid.
     */
    EntitySpatialGridPartition(const float &scale);

    /**
     * @brief Destructor for the EntitySpatialGridPartition class.
     */
    ~EntitySpatialGridPartition();

    /**
     * @brief Retrieves a cell from the spatial grid partition.
     *
     * This method provides direct access to a cell at the given (x, y) coordinates.
     *
     * @param x The x-coordinate of the cell.
     * @param y The y-coordinate of the cell.
     * @return A reference to the cell at the given coordinates.
     */
    Cell &getCell(const int &x, const int &y);

    /**
     * @brief Retrieves the entity look-up table.
     *
     * The look-up table maps each entity's unique ID to its corresponding grid coordinates.
     *
     * @return A constant reference to the entity look-up table.
     */
    const std::unordered_map<uint64_t, sf::Vector2i> &getEntityLookUpTable() const;

    /**
     * @brief Retrieves the spatial grid coordinates of a given entity.
     *
     * If the entity is not found in the spatial grid, a warning is logged, and (-1, -1) is returned.
     *
     * @param entity A shared pointer to the entity whose coordinates are being retrieved.
     * @return The spatial grid coordinates (x, y) of the entity.
     */
    const sf::Vector2i getEntitySpatialGridCoords(std::shared_ptr<Entity> entity);

    /**
     * @brief Calculates the spatial grid coordinates of a given entity.
     *
     * This method computes the grid coordinates based on the entity's position or hitbox.
     *
     * @param entity A shared pointer to the entity whose coordinates are being calculated.
     * @return The calculated spatial grid coordinates (x, y).
     */
    sf::Vector2i calculateSpatialGridCoords(std::shared_ptr<Entity> entity) const;

    /**
     * @brief Resizes the spatial grid partition to the given dimensions.
     *
     * This method resizes the grid to fit the specified number of rows and columns of cells.
     *
     * @param x The new number of rows in the grid.
     * @param y The new number of columns in the grid.
     */
    void resize(const unsigned int x, const unsigned int y);

    /**
     * @brief Adds an entity to the spatial grid partition.
     *
     * This method places the given entity in the appropriate cell based on its spatial grid coordinates.
     * If the entity already exists in the grid, it will not be added.
     *
     * @param entity A shared pointer to the entity to be added.
     * @return True if the entity was successfully added, otherwise false.
     */
    const bool put(std::shared_ptr<Entity> entity);

    /**
     * @brief Removes an entity from the spatial grid partition.
     *
     * This method removes the specified entity from its current cell in the grid.
     *
     * @param entity A shared pointer to the entity to be removed.
     * @return True if the entity was successfully removed, otherwise false.
     */
    const bool remove(std::shared_ptr<Entity> entity);

    /**
     * @brief Moves an entity to a different cell in the spatial grid.
     *
     * This method removes the entity from its current cell and places it in the specified new cell.
     * If the entity cannot be moved, the method returns false.
     *
     * @param entity A shared pointer to the entity to be moved.
     * @param to_x The target x-coordinate of the new cell.
     * @param to_y The target y-coordinate of the new cell.
     * @return True if the entity was successfully moved, otherwise false.
     */
    const bool move(std::shared_ptr<Entity> entity, const int &to_x, const int &to_y);

    /**
     * @brief Checks if an entity exists in the entity look-up table.
     *
     * This method checks if the given entity has been added to the spatial grid partition.
     *
     * @param entity A shared pointer to the entity.
     * @return True if the entity exists in the look-up table, otherwise false.
     */
    const bool existsInTable(std::shared_ptr<Entity> entity);
};
