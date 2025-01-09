#pragma once

#include "Engine/Configuration.hxx"
#include "Tiles/Tile.hxx"

class Chunk
{
  public:
    sf::RectangleShape chunkBorders;
    sf::Vector2<unsigned int> chunkIndex;
    unsigned int gridSize;
    float scale;
    std::array<std::array<std::array<std::unique_ptr<TileBase>, CHUNK_SIZE.z>, CHUNK_SIZE.y>, CHUNK_SIZE.x> tiles;

    Chunk(const sf::Vector2u chunk_index, const unsigned int &grid_size, const float &scale);
    ~Chunk();

    void update(const float &dt);

    void render(sf::RenderTarget &target, const bool show_chunk_borders = false);
};
