#include "Map/Chunk.hxx"
#include "stdafx.hxx"

void Chunk::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &texturePack;
    target.draw(vertices, states);
}

Chunk::Chunk(sf::Texture &texture_pack, const sf::Vector2u chunk_index, const float &scale, uint8_t flags)
    : texturePack(texture_pack), chunkIndex(chunk_index), scale(scale), flags(flags)
{
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);

    vertices.resize(CHUNK_SIZE_IN_TILES.x * CHUNK_SIZE_IN_TILES.y * CHUNK_SIZE_IN_TILES.z * QUAD_VERTEX_COUNT);

    chunkBorders.setSize(
        sf::Vector2f(CHUNK_SIZE_IN_TILES.x * GRID_SIZE * scale, CHUNK_SIZE_IN_TILES.y * GRID_SIZE * scale));
    chunkBorders.setPosition(sf::Vector2f(chunk_index.x * CHUNK_SIZE_IN_TILES.x * GRID_SIZE * scale,
                                          chunk_index.y * CHUNK_SIZE_IN_TILES.y * GRID_SIZE * scale));
    chunkBorders.setFillColor(sf::Color::Transparent);
    chunkBorders.setOutlineThickness(-1.f);
    chunkBorders.setOutlineColor(sf::Color::Green);
}

Chunk::~Chunk() = default;

void Chunk::update(const float &dt)
{
    updateVertexArray();
}

void Chunk::updateVertexArray()
{
    vertices.clear();

    for (auto &x : tiles)
    {
        for (auto &y : x)
        {
            for (auto &z : y)
            {
                if (z)
                {
                    sf::VertexArray tile_vertices = z->getVertexArray();

                    for (int i = 0; i < QUAD_VERTEX_COUNT; i++)
                        vertices.append(tile_vertices[i]);
                }
            }
        }
    }
}
