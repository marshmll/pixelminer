#include "Map/EntitySpatialGridPartition.hxx"
#include "stdafx.hxx"

EntitySpatialGridPartition::EntitySpatialGridPartition(const float &scale) : logger("EntitySpatialGridPartition")
{
    resize(SPATIAL_GRID_PARTITION_DIMENSIONS.x, SPATIAL_GRID_PARTITION_DIMENSIONS.y);
}

EntitySpatialGridPartition::~EntitySpatialGridPartition() = default;

Cell &EntitySpatialGridPartition::getCell(const int &x, const int &y)
{
    if (x < 0 || x >= SPATIAL_GRID_PARTITION_DIMENSIONS.x || y < 0 || y >= SPATIAL_GRID_PARTITION_DIMENSIONS.y)
        logger.logError("Cell out of bounds: " + std::to_string(x) + ", " + std::to_string(y));

    return cells[x][y];
}

const std::unordered_map<uint64_t, sf::Vector2i> &EntitySpatialGridPartition::getEntityLookUpTable() const
{
    return entitySpacialGridLookUpTable;
}

const sf::Vector2i EntitySpatialGridPartition::getEntitySpatialGridCoords(std::shared_ptr<Entity> entity)
{
    if (entitySpacialGridLookUpTable.count(entity->getId()) == 0)
    {
        logger.logWarning("Entiity " + entity->getName() + " with id " + std::to_string(entity->getId()) +
                          " was not found in the spatial grid partition.");

        return sf::Vector2i(-1, -1);
    }

    return entitySpacialGridLookUpTable.at(entity->getId());
}

sf::Vector2i EntitySpatialGridPartition::calculateSpatialGridCoords(std::shared_ptr<Entity> entity) const
{
    sf::Vector2i coords;

    if (entity->isCollideable())
    {
        coords.x = static_cast<int>(entity->getFirstHitBoxGridPosition().x / SPATIAL_GRID_PARTITION_CELL_SIZE_IN_TILES);
        coords.y = static_cast<int>(entity->getFirstHitBoxGridPosition().y / SPATIAL_GRID_PARTITION_CELL_SIZE_IN_TILES);
    }
    else
    {
        coords.x = static_cast<int>(entity->getCenterGridPosition().x / SPATIAL_GRID_PARTITION_CELL_SIZE_IN_TILES);
        coords.y = static_cast<int>(entity->getCenterGridPosition().y / SPATIAL_GRID_PARTITION_CELL_SIZE_IN_TILES);
    }

    return coords;
}

void EntitySpatialGridPartition::resize(const unsigned int x, const unsigned int y)
{
    cells.resize(x);

    for (auto &row : cells)
        row.resize(y);
}

const bool EntitySpatialGridPartition::put(std::shared_ptr<Entity> entity)
{
    if (!entity.get())
        return false;

    if (existsInTable(entity))
        return false;

    auto [cell_x, cell_y] = calculateSpatialGridCoords(entity);

    if (cell_x < 0 || cell_x >= SPATIAL_GRID_PARTITION_DIMENSIONS.x || cell_y < 0 ||
        cell_y >= SPATIAL_GRID_PARTITION_DIMENSIONS.y)
        return false;

    cells[cell_x][cell_y].emplace_back(entity);
    entitySpacialGridLookUpTable[entity->getId()] = sf::Vector2i(cell_x, cell_y);

    return true;
}

const bool EntitySpatialGridPartition::remove(std::shared_ptr<Entity> entity)
{
    if (!entity.get())
        return false;

    if (!existsInTable(entity))
        return false;

    auto [cell_x, cell_y] = getEntitySpatialGridCoords(entity);

    auto it = std::find_if(cells[cell_x][cell_y].begin(), cells[cell_x][cell_y].end(),
                           [&](std::shared_ptr<Entity> e) { return e.get() == entity.get(); });

    if (it == cells[cell_x][cell_y].end())
        return false;

    cells[cell_x][cell_y].erase(it);
    entitySpacialGridLookUpTable.erase(entity->getId());

    return true;
}

const bool EntitySpatialGridPartition::move(std::shared_ptr<Entity> entity, const int &to_x, const int &to_y)
{
    if (!entity.get())
        return false;

    if (to_x < 0 || to_x >= SPATIAL_GRID_PARTITION_DIMENSIONS.x || to_y < 0 ||
        to_y >= SPATIAL_GRID_PARTITION_DIMENSIONS.y)
        return false;

    if (!existsInTable(entity))
        return false;

    auto [from_x, from_y] = getEntitySpatialGridCoords(entity);

    auto it = std::find_if(cells[from_x][from_y].begin(), cells[from_x][from_y].end(),
                           [&](std::shared_ptr<Entity> e) { return e.get() == entity.get(); });

    if (it == cells[from_x][from_y].end())
        return false;

    if (!remove(*it))
        return false;

    if (!put(entity))
        return false;

    return true;
}

const bool EntitySpatialGridPartition::existsInTable(std::shared_ptr<Entity> entity)
{
    if (!entity.get())
        return false;

    return entitySpacialGridLookUpTable.count(entity->getId()) > 0;
}
