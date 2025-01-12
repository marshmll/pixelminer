#pragma once

#include "Map/Chunk.hxx"

static constexpr sf::Vector2u REGION_SIZE_IN_CHUNKS = {2, 2};

class Region
{
  public:
    sf::RectangleShape regionBorders;
    std::array<std::array<std::unique_ptr<Chunk>, REGION_SIZE_IN_CHUNKS.y>, REGION_SIZE_IN_CHUNKS.x> chunks;
    sf::Vector2<unsigned int> regionIndex;
    unsigned int gridSize;
    float scale;

    Region(const sf::Vector2u region_index, const unsigned int &grid_size, const float &scale);
    ~Region();

    void update(const float &dt);

    void render(sf::RenderTarget &target, const bool show_regions_and_chunks = false);
};