#pragma once

#include "Engine/Configuration.hxx"
#include "Entities/Entity.hxx"
#include "Map/TerrainGenerator.hxx"

constexpr unsigned int SPACIAL_GRID_PARTITION_CELL_SIZE_IN_TILES = 8;

static const sf::Vector2u SPACIAL_GRID_PARTITION_DIMENSIONS =
    sf::Vector2u(std::ceil(static_cast<float>(MAX_WORLD_GRID_SIZE.x) /
                           static_cast<float>(SPACIAL_GRID_PARTITION_CELL_SIZE_IN_TILES)),
                 std::ceil(static_cast<float>(MAX_WORLD_GRID_SIZE.y) /
                           static_cast<float>(SPACIAL_GRID_PARTITION_CELL_SIZE_IN_TILES)));

using Cell = std::vector<std::shared_ptr<Entity>>;
using SpacialGridPartition = std::vector<std::vector<Cell>>;

class EntitySpacialGridPartition
{
  private:
    Logger logger;
    SpacialGridPartition cells;
    std::unordered_map<uint64_t, sf::Vector2i> entitySpacialGridLookUpTable;
    float scale;

  public:
    EntitySpacialGridPartition(const float &scale);

    ~EntitySpacialGridPartition();

    // DO NOT INSERT OR REMOVE DIRECTLY! USE ONLY TO ACCESS ENTITIES!
    Cell &getCell(const int &x, const int &y);

    const std::unordered_map<uint64_t, sf::Vector2i> &getEntityLookUpTable() const;

    const sf::Vector2i getEntitySpatialGridCoords(std::shared_ptr<Entity> entity);

    sf::Vector2i calculateSpatialGridCoords(std::shared_ptr<Entity> entity) const;

    void resize(const unsigned int x, const unsigned int y);

    const bool put(std::shared_ptr<Entity> entity);

    const bool remove(std::shared_ptr<Entity> entity);

    const bool move(std::shared_ptr<Entity> entity, const int &to_x, const int &to_y);

    const bool existsInTable(std::shared_ptr<Entity> entity);
};