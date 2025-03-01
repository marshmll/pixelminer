#include "Tiles/TileBase.hxx"
#include "stdafx.hxx"

TileBase::TileBase(const std::string &name, const std::string &tag, const uint64_t &id, sf::Texture &texture,
                   const sf::IntRect &texture_rect, const sf::Vector2i &grid_position, const float &scale)
    : name(name), tag(tag), id(id), texture(texture), textureRect(texture_rect), scaleScalar(scale)
{
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    vertices.resize(QUAD_VERTEX_COUNT);

    sf::Vector2f pos(grid_position.x * GRID_SIZE * scaleScalar, grid_position.y * GRID_SIZE * scaleScalar);
    float size = GRID_SIZE * scaleScalar;

    // define the 6 corners of the two triangles
    vertices[0].position = sf::Vector2f(pos);
    vertices[1].position = sf::Vector2f(pos.x + size, pos.y);
    vertices[2].position = sf::Vector2f(pos.x, pos.y + size);
    vertices[3].position = sf::Vector2f(pos.x, pos.y + size);
    vertices[4].position = sf::Vector2f(pos.x + size, pos.y);
    vertices[5].position = sf::Vector2f(pos.x + size, pos.y + size);

    // define the 6 matching texture coordinates
    vertices[0].texCoords = sf::Vector2f(textureRect.position);
    vertices[1].texCoords = sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y);
    vertices[2].texCoords = sf::Vector2f(textureRect.position.x, textureRect.position.y + textureRect.size.y);
    vertices[3].texCoords = sf::Vector2f(textureRect.position.x, textureRect.position.y + textureRect.size.y);
    vertices[4].texCoords = sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y);
    vertices[5].texCoords =
        sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y + textureRect.size.y);
}

TileBase::~TileBase() = default;

void TileBase::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &texture;
    target.draw(vertices, states);
}

const std::string &TileBase::getName() const
{
    return name;
}

const std::string &TileBase::getTag() const
{
    return tag;
}

const uint64_t &TileBase::getId() const
{
    return id;
}

const sf::Vector2f TileBase::getPosition() const
{
    return vertices[0].position;
}

const sf::Vector2u TileBase::getGridPosition() const
{
    return sf::Vector2u(static_cast<unsigned>(vertices[0].position.x / (GRID_SIZE * scaleScalar)),
                        static_cast<unsigned>(vertices[0].position.y / (GRID_SIZE * scaleScalar)));
}

sf::Color TileBase::getColor() const
{
    return vertices[0].color;
}

const sf::FloatRect TileBase::getGlobalBounds() const
{
    sf::FloatRect global_bounds;
    global_bounds.position = vertices[0].position;
    global_bounds.size = sf::Vector2f(GRID_SIZE * scaleScalar, GRID_SIZE * scaleScalar);

    return global_bounds;
}

const sf::Vector2f TileBase::getCenter() const
{
    return sf::Vector2f(vertices[0].position.x + static_cast<float>(GRID_SIZE * scaleScalar) / 2.f,
                        vertices[0].position.y + static_cast<float>(GRID_SIZE * scaleScalar) / 2.f);
}

const sf::VertexArray &TileBase::getVertexArray() const
{
    return vertices;
}

void TileBase::setPosition(const sf::Vector2f &position)
{
    float size = GRID_SIZE * scaleScalar;

    vertices[0].position = sf::Vector2f(position);
    vertices[1].position = sf::Vector2f(position.x + size, position.y);
    vertices[2].position = sf::Vector2f(position.x, position.y + size);
    vertices[3].position = sf::Vector2f(position.x, position.y + size);
    vertices[4].position = sf::Vector2f(position.x + size, position.y);
    vertices[5].position = sf::Vector2f(position.x + size, position.y + size);
}

void TileBase::setGridPosition(const sf::Vector2u &position)
{
    sf::Vector2f pos(position.x * GRID_SIZE * scaleScalar, position.y * GRID_SIZE * scaleScalar);
    float size = GRID_SIZE * scaleScalar;

    vertices[0].position = sf::Vector2f(pos);
    vertices[1].position = sf::Vector2f(pos.x + size, pos.y);
    vertices[2].position = sf::Vector2f(pos.x, pos.y + size);
    vertices[3].position = sf::Vector2f(pos.x, pos.y + size);
    vertices[4].position = sf::Vector2f(pos.x + size, pos.y);
    vertices[5].position = sf::Vector2f(pos.x + size, pos.y + size);
}

void TileBase::setColor(const sf::Color &color)
{
    for (int i = 0; i < QUAD_VERTEX_COUNT; i++)
        vertices[i].color = color;
}
