#pragma once

#include "Engine/Configuration.hxx"
#include "Tiles/TileBase.hxx"

static constexpr sf::Vector3<unsigned int> CHUNK_SIZE_IN_TILES = {16, 16, 5};

class Chunk
{
  public:
    sf::RectangleShape chunkBorders;
    sf::Vector2<unsigned int> chunkIndex;
    unsigned int gridSize;
    float scale;
    std::array<std::array<std::array<std::unique_ptr<TileBase>, CHUNK_SIZE_IN_TILES.z>, CHUNK_SIZE_IN_TILES.y>, CHUNK_SIZE_IN_TILES.x> tiles;

    Chunk(const sf::Vector2u chunk_index, const unsigned int &grid_size, const float &scale);
    ~Chunk();

    void update(const float &dt);

    void render(sf::RenderTarget &target, const bool show_chunk_borders = false);
};
