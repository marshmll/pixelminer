#include "Map/EntitySpatialGridPartition.hxx"
#include "stdafx.hxx"

EntitySpatialGridPartition::EntitySpatialGridPartition(const float &scale)
    : logger("EntitySpatialGridPartition"), scale(scale)
{
    resize(SPATIAL_GRID_PARTITION_DIMENSIONS.x, SPATIAL_GRID_PARTITION_DIMENSIONS.y);
}

EntitySpatialGridPartition::~EntitySpatialGridPartition() = default;

Cell &EntitySpatialGridPartition::getCell(const int &x, const int &y)
{
    if (x < 0 || x >= SPATIAL_GRID_PARTITION_DIMENSIONS.x || y < 0 || y >= SPATIAL_GRID_PARTITION_DIMENSIONS.y)
    {
        logger.logError("Cell out of bounds: " + std::to_string(x) + ", " + std::to_string(y));
        throw std::out_of_range("Cell coordinates out of bounds");
    }
    return cells[x][y];
}

const std::unordered_map<uint64_t, sf::Vector2i> &EntitySpatialGridPartition::getEntityLookUpTable() const
{
    return entitySpacialGridLookUpTable;
}

const sf::Vector2i EntitySpatialGridPartition::getEntityCellGridCoords(const std::shared_ptr<Entity> &entity)
{
    const auto it = entitySpacialGridLookUpTable.find(entity->getId());
    if (it == entitySpacialGridLookUpTable.end())
    {
        logger.logWarning("Entity " + entity->getName() + " with id " + std::to_string(entity->getId()) +
                          " was not found in the spatial grid partition.");
        return {};
    }
    return it->second;
}

const sf::Vector2i EntitySpatialGridPartition::calcEntityCellGridCoords(const std::shared_ptr<Entity> &entity) const
{
    const float gridSizeScaled = GRID_SIZE * scale;
    const float cellSize = SPATIAL_GRID_PARTITION_CELL_SIZE_IN_TILES;

    // Calculate the center position of the entity
    sf::Vector2f center_pos;
    if (entity->isCollideable())
    {
        const auto &hitBoxPos = entity->getFirstHitBoxPosition();
        const auto &hitBoxSize = entity->getFirstHitBoxSize();
        center_pos = sf::Vector2f(hitBoxPos.x + hitBoxSize.x / 2, hitBoxPos.y + hitBoxSize.y / 2);
    }
    else
    {
        const auto &pos = entity->getPosition();
        const auto &size = entity->getSize();
        center_pos = sf::Vector2f(pos.x + size.x / 2, pos.y + size.y / 2);
    }

    // Calculate the cell coordinates based on the reference position.
    const sf::Vector2i cell_coords(static_cast<int>(center_pos.x / gridSizeScaled / cellSize),
                                   static_cast<int>(center_pos.y / gridSizeScaled / cellSize));

    return std::move(cell_coords);
}

void EntitySpatialGridPartition::resize(const unsigned int x, const unsigned int y)
{
    cells.resize(x);
    for (auto &row : cells)
        row.resize(y);
}

const bool EntitySpatialGridPartition::put(const std::shared_ptr<Entity> &entity)
{
    if (!entity || existsInTable(entity))
        return false;

    const sf::Vector2i cell_grid_coords = calcEntityCellGridCoords(entity);
    const auto &[cell_x, cell_y] = cell_grid_coords;

    if (cell_x < 0 || cell_x >= SPATIAL_GRID_PARTITION_DIMENSIONS.x || cell_y < 0 ||
        cell_y >= SPATIAL_GRID_PARTITION_DIMENSIONS.y)
        return false;

    cells[cell_x][cell_y].emplace_back(entity);
    entitySpacialGridLookUpTable.emplace(entity->getId(), std::move(cell_grid_coords));
    return true;
}

const bool EntitySpatialGridPartition::remove(const std::shared_ptr<Entity> &entity)
{
    if (!entity || !existsInTable(entity))
        return false;

    const sf::Vector2i cell_grid_coords = getEntityCellGridCoords(entity);
    const auto &[cell_x, cell_y] = cell_grid_coords;

    Cell &cell = cells[cell_x][cell_y];

    bool erased_any = false;
    auto it = std::remove_if(cell.begin(), cell.end(), [&](const std::shared_ptr<Entity> &e) { return e == entity; });
    if (it != cell.end())
    {
        cell.erase(it, cell.end());
        erased_any = true;
    }

    if (erased_any)
        entitySpacialGridLookUpTable.erase(entity->getId());

    return erased_any;
}

const bool EntitySpatialGridPartition::move(const std::shared_ptr<Entity> &entity, const sf::Vector2i &new_cell)
{
    if (!entity || !existsInTable(entity))
        return false;

    // Remove the entity from its current cell
    if (!remove(entity))
        return false;

    const auto &[cell_x, cell_y] = new_cell;

    if (cell_x < 0 || cell_x >= SPATIAL_GRID_PARTITION_DIMENSIONS.x || cell_y < 0 ||
        cell_y >= SPATIAL_GRID_PARTITION_DIMENSIONS.y)
        return false;

    cells[cell_x][cell_y].emplace_back(entity);
    entitySpacialGridLookUpTable[entity->getId()] = new_cell;
    return true;
}

const bool EntitySpatialGridPartition::existsInTable(const std::shared_ptr<Entity> &entity) const
{
    return entity && entitySpacialGridLookUpTable.count(entity->getId()) > 0;
}