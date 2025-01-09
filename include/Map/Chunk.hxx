#pragma once

#include "Tiles/TileBase.hxx"

static constexpr sf::Vector3i CHUNK_SIZE = {20, 20, 5};

class Chunk
{
  public:
    sf::RectangleShape chunkBorders;
    sf::Vector2<unsigned int> gridCoords;
    std::array<std::array<std::array<std::unique_ptr<TileBase>, CHUNK_SIZE.z>, CHUNK_SIZE.y>, CHUNK_SIZE.x> tiles;

    Chunk(const sf::Vector2u grid_coords, const unsigned int &grid_size, const float &scale);
    ~Chunk();

    void update(const float &dt);

    void render(sf::RenderTarget &target, const bool show_chunk_borders = false);
};