#include "Map/EntitySpacialGridPartition.hxx"
#include "stdafx.hxx"

EntitySpacialGridPartition::EntitySpacialGridPartition() : logger("EntitySpacialGridPartition")
{
    resize(SPACIAL_GRID_PARTITION_DIMENSIONS.x, SPACIAL_GRID_PARTITION_DIMENSIONS.y);
}

EntitySpacialGridPartition::~EntitySpacialGridPartition()
{
}

const Cell &EntitySpacialGridPartition::getCell(const int &x, const int &y)
{
    if (x < 0 || x >= SPACIAL_GRID_PARTITION_DIMENSIONS.x || y < 0 || y >= SPACIAL_GRID_PARTITION_DIMENSIONS.y)
        logger.logError("Cell out of bounds: " + std::to_string(x) + ", " + std::to_string(y));

    return cells[x][y];
}

const std::unordered_map<uint64_t, sf::Vector2i> &EntitySpacialGridPartition::getEntityLookUpTable() const
{
    return entityLookUpTable;
}

const sf::Vector2i EntitySpacialGridPartition::getEntitySpatialGridCoords(std::shared_ptr<Entity> entity)
{
    if (entityLookUpTable.count(entity->getId()) == 0)
    {
        logger.logWarning("Entiity " + entity->getName() + " with id " + std::to_string(entity->getId()) +
                          " was not found in the spatial grid partition.");

        return sf::Vector2i(-1, -1);
    }

    return entityLookUpTable.at(entity->getId());
}

sf::Vector2i EntitySpacialGridPartition::calculateSpatialGridCoords(std::shared_ptr<Entity> entity) const
{
    sf::Vector2i coords;

    coords.x = static_cast<int>(entity->getCenterGridPosition().x / SPACIAL_GRID_PARTITION_CELL_SIZE_IN_TILES);
    coords.y = static_cast<int>(entity->getCenterGridPosition().y / SPACIAL_GRID_PARTITION_CELL_SIZE_IN_TILES);

    return coords;
}

void EntitySpacialGridPartition::resize(const unsigned int x, const unsigned int y)
{
    cells.resize(x);

    for (auto &row : cells)
        row.resize(y);
}

const bool EntitySpacialGridPartition::put(std::shared_ptr<Entity> entity)
{
    if (!entity.get())
        return false;

    if (existsInTable(entity))
        return false;

    auto [cell_x, cell_y] = calculateSpatialGridCoords(entity);

    if (cell_x < 0 || cell_x >= SPACIAL_GRID_PARTITION_DIMENSIONS.x || cell_y < 0 ||
        cell_y >= SPACIAL_GRID_PARTITION_DIMENSIONS.y)
        return false;

    cells[cell_x][cell_y].emplace_back(entity);
    entityLookUpTable[entity->getId()] = {cell_x, cell_y};

    return true;
}

const bool EntitySpacialGridPartition::remove(std::shared_ptr<Entity> entity)
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
    entityLookUpTable.erase(entity->getId());

    return true;
}

const bool EntitySpacialGridPartition::move(std::shared_ptr<Entity> entity, const int &to_x, const int &to_y)
{
    if (!entity.get())
        return false;

    if (to_x < 0 || to_x >= SPACIAL_GRID_PARTITION_DIMENSIONS.x || to_y < 0 ||
        to_y >= SPACIAL_GRID_PARTITION_DIMENSIONS.y)
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

const bool EntitySpacialGridPartition::existsInTable(std::shared_ptr<Entity> entity)
{
    if (!entity.get())
        return false;

    return entityLookUpTable.count(entity->getId()) > 0;
}
