#include "Map/Region.hxx"
#include "stdafx.hxx"

Region::Region(const sf::Vector2u region_index, const unsigned int &grid_size, const float &scale)
    : regionIndex(region_index), gridSize(grid_size), scale(scale)
{
    regionBorders.setSize(sf::Vector2f(REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x * grid_size * scale,
                                       REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y * grid_size * scale));

    regionBorders.setPosition(sf::Vector2f(region_index.x * REGION_SIZE_IN_CHUNKS.x * CHUNK_SIZE_IN_TILES.x * grid_size * scale,
                                           region_index.y * REGION_SIZE_IN_CHUNKS.y * CHUNK_SIZE_IN_TILES.y * grid_size * scale));

    regionBorders.setFillColor(sf::Color::Transparent);
    regionBorders.setOutlineThickness(-1.f);
    regionBorders.setOutlineColor(sf::Color::Magenta);
}

Region::~Region()
{
}

void Region::update(const float &dt)
{
    for (auto &row : chunks)
    {
        for (auto &chunk : row)
        {
            if (chunk)
                chunk->update(dt);
        }
    }
}

void Region::render(sf::RenderTarget &target, const bool show_regions_and_chunks)
{
    for (auto &row : chunks)
    {
        for (auto &chunk : row)
        {
            if (chunk)
                chunk->render(target, show_regions_and_chunks);
        }
    }

    if (show_regions_and_chunks)
        target.draw(regionBorders);
}