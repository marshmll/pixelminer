#include "Tiles/TileBase.hxx"
#include "stdafx.hxx"

TileBase::TileBase(const std::string &name, const std::string &tag, const uint64_t &id, sf::Texture &texture,
                   const sf::IntRect &texture_rect, const float &scale)
    : name(name), tag(tag), id(id), texture(texture), textureRect(texture_rect), sprite(texture, textureRect)
{
    sprite.setScale(sf::Vector2f(scale, scale));
}

TileBase::~TileBase() = default;

void TileBase::render(sf::RenderTarget &target)
{
    target.draw(sprite);
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
    return sprite.getPosition();
}

const sf::Vector2u TileBase::getGridPosition() const
{
    return sf::Vector2u(static_cast<unsigned>(sprite.getPosition().x / (GRID_SIZE * sprite.getScale().x)),
                        static_cast<unsigned>(sprite.getPosition().y / (GRID_SIZE * sprite.getScale().y)));
}

sf::Color TileBase::getColor() const
{
    return sprite.getColor();
}

const sf::FloatRect TileBase::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}

const sf::Vector2f TileBase::getCenter() const
{
    return sprite.getGlobalBounds().getCenter();
}

void TileBase::getTriangles(sf::Vertex triangles[6])
{
    sf::Vector2f pos = getPosition();
    float size = GRID_SIZE * sprite.getScale().x;

    // define the 6 corners of the two triangles
    triangles[0].position = sf::Vector2f(pos);
    triangles[1].position = sf::Vector2f(pos.x + size, pos.y);
    triangles[2].position = sf::Vector2f(pos.x, pos.y + size);
    triangles[3].position = sf::Vector2f(pos.x, pos.y + size);
    triangles[4].position = sf::Vector2f(pos.x + size, pos.y);
    triangles[5].position = sf::Vector2f(pos.x + size, pos.y + size);

    // define the 6 matching texture coordinates
    triangles[0].texCoords = sf::Vector2f(textureRect.position);
    triangles[1].texCoords = sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y);
    triangles[2].texCoords = sf::Vector2f(textureRect.position.x, textureRect.position.y + textureRect.size.y);
    triangles[3].texCoords = sf::Vector2f(textureRect.position.x, textureRect.position.y + textureRect.size.y);
    triangles[4].texCoords = sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y);
    triangles[5].texCoords =
        sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y + textureRect.size.y);

    for (int i = 0; i < QUAD_VERTEX_COUNT; i++)
        triangles[i].color = sprite.getColor();
}

void TileBase::setPosition(const sf::Vector2f &position)
{
    sprite.setPosition(position);
}

void TileBase::setGridPosition(const sf::Vector2u &position)
{
    sprite.setPosition(sf::Vector2f(std::floor(position.x * GRID_SIZE * sprite.getScale().x),
                                    std::floor(position.y * GRID_SIZE * sprite.getScale().y)));
}

void TileBase::setColor(const sf::Color &color)
{
    sprite.setColor(color);
}
