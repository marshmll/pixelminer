#include "Map/Chunk.hxx"
#include "stdafx.hxx"

Chunk::Chunk(const sf::Vector2u chunk_index, const unsigned int &grid_size, const float &scale, uint8_t flags)
    : chunkIndex(chunk_index), gridSize(grid_size), scale(scale), flags(flags)
{
    chunkBorders.setSize(
        sf::Vector2f(CHUNK_SIZE_IN_TILES.x * grid_size * scale, CHUNK_SIZE_IN_TILES.y * grid_size * scale));
    chunkBorders.setPosition(sf::Vector2f(chunk_index.x * CHUNK_SIZE_IN_TILES.x * grid_size * scale,
                                          chunk_index.y * CHUNK_SIZE_IN_TILES.y * grid_size * scale));
    chunkBorders.setFillColor(sf::Color::Transparent);
    chunkBorders.setOutlineThickness(-1.f);
    chunkBorders.setOutlineColor(sf::Color::Green);
}

Chunk::~Chunk() = default;

void Chunk::update(const float &dt)
{}

void Chunk::render(sf::RenderTarget &target, const bool &debug)
{
    for (auto &x : tiles)
    {
        for (auto &y : x)
        {
            for (auto &z : y)
            {
                if (z)
                    z->render(target);
            }
        }
    }

    if (debug)
        target.draw(chunkBorders);
}