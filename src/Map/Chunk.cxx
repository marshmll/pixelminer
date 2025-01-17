#include "Map/Chunk.hxx"
#include "stdafx.hxx"

Chunk::Chunk(const sf::Vector2u chunk_index, const unsigned int &grid_size, const float &scale)
    : chunkIndex(chunk_index), gridSize(grid_size), scale(scale)
{
    chunkBorders.setSize(
        sf::Vector2f(CHUNK_SIZE_IN_TILES.x * grid_size * scale, CHUNK_SIZE_IN_TILES.y * grid_size * scale));
    chunkBorders.setPosition(sf::Vector2f(chunk_index.x * CHUNK_SIZE_IN_TILES.x * grid_size * scale,
                                          chunk_index.y * CHUNK_SIZE_IN_TILES.y * grid_size * scale));
    chunkBorders.setFillColor(sf::Color::Transparent);
    chunkBorders.setOutlineThickness(-1.f);
    chunkBorders.setOutlineColor(sf::Color::Green);
}

Chunk::~Chunk()
{
}

void Chunk::update(const float &dt)
{
}

void Chunk::render(sf::RenderTarget &target, const bool &debug)
{
    for (int x = 0; x < CHUNK_SIZE_IN_TILES.x; x++)
    {
        for (int y = 0; y < CHUNK_SIZE_IN_TILES.y; y++)
        {
            for (int z = 0; z < CHUNK_SIZE_IN_TILES.z; z++)
            {
                if (tiles[x][y][z].get())
                    tiles[x][y][z]->render(target);
            }
        }
    }

    if (debug)
        target.draw(chunkBorders);
}