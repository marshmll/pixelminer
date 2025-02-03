#include "Entities/Inanimated/Trees/Tree.hxx"
#include "stdafx.hxx"

Tree::Tree(const std::string name, const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet,
           const float &scale)
    : Entity(name, spawn_grid_position, sprite_sheet, scale)
{
}

Tree::~Tree() = default;
