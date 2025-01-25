#pragma once

#include "Engine/Configuration.hxx"
#include "Tiles/Tile.hxx"

enum ChunkFlags : uint8_t
{
    None = 0x0,       // No flags
    Modified = 0x1,   // The chunk is different from default generation
    KeepLoaded = 0x2, // Do not unload from memory
};

static constexpr sf::Vector2u REGION_SIZE_IN_CHUNKS = {6, 6};
static constexpr sf::Vector3<unsigned int> CHUNK_SIZE_IN_TILES = {16, 16, 5};

using TileMatrix =
    std::array<std::array<std::array<std::unique_ptr<Tile>, CHUNK_SIZE_IN_TILES.z>, CHUNK_SIZE_IN_TILES.y>,
               CHUNK_SIZE_IN_TILES.x>;

class Chunk
{
  public:
    sf::RectangleShape chunkBorders;
    sf::Vector2<unsigned int> chunkIndex;

    unsigned int gridSize;
    float scale;

    TileMatrix tiles;

    uint8_t flags;

    Chunk(const sf::Vector2u chunk_index, const unsigned int &grid_size, const float &scale, uint8_t flags = ChunkFlags::None);

    ~Chunk();

    void update(const float &dt);

    void render(sf::RenderTarget &target, const bool &debug = false);
};
