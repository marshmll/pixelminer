#include "Map/Chunk.hxx"
#include "stdafx.hxx"

Chunk::Chunk(const sf::Vector2u grid_coords, const unsigned int &grid_size, const float &scale)
    : gridCoords(grid_coords)
{
    chunkBorders.setSize(sf::Vector2f(CHUNK_SIZE.x * grid_size * scale, CHUNK_SIZE.y * grid_size * scale));
    chunkBorders.setPosition(sf::Vector2f(grid_coords.x * CHUNK_SIZE.x * grid_size * scale,
                                          grid_coords.y * CHUNK_SIZE.y * grid_size * scale));
    chunkBorders.setFillColor(sf::Color(0, 100, 0, 20));
    chunkBorders.setOutlineThickness(-1.f);
    chunkBorders.setOutlineColor(sf::Color::Green);
}

Chunk::~Chunk()
{
}

void Chunk::update(const float &dt)
{
}

void Chunk::render(sf::RenderTarget &target, const bool show_chunk_borders)
{
    for (auto &x : tiles)
    {
        for (auto &y : x)
        {
            for (auto &tile : y)
            {
                if (tile)
                    tile->render(target);
            }
        }
    }

    if (show_chunk_borders)
        target.draw(chunkBorders);
}