#include "Entities/Inanimated/Trees/PineTree.hxx"
#include "stdafx.hxx"

void PineTree::initAnimations()
{
    animationFunctionality->addAnimation("Idle", 10000, {50, 82}, {0, 0}, {0, 0});
}

PineTree::PineTree(const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet, const float &scale)
    : Tree("Pine Tree", spawn_grid_position, sprite_sheet, scale)
{
    createAnimationFunctionality();
    initAnimations();
}

PineTree::~PineTree() = default;

void PineTree::update(const float &dt, const sf::Vector2f &mouse_pos)
{
    animationFunctionality->play("Idle");

    if (sprite.getGlobalBounds().contains(mouse_pos))
        std::cout << "idling" << "\n";
}

void PineTree::render(sf::RenderTarget &target)
{
    target.draw(sprite);
}
