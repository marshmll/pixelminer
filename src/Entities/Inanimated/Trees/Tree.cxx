#include "Entities/Inanimated/Trees/Tree.hxx"
#include "stdafx.hxx"

Tree::Tree(const std::string name, const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet,
           const float &scale, std::unordered_map<std::string, sf::SoundBuffer> &sound_buffers)
    : Entity(name, EntityType::InanimatedEntity, spawn_grid_position, sprite_sheet, scale, sound_buffers,
             RenderBehavior::Perspective)
{}

Tree::~Tree() = default;
