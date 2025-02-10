#include "Entities/Inanimated/Trees/PineTree.hxx"
#include "stdafx.hxx"

void PineTree::initAnimations()
{
    animationFunctionality->addAnimation("Top", "CrownIdle", 10000, {50, 82}, {0, 0}, {0, 0});
    animationFunctionality->addAnimation("Base", "TrunkIdle", 10000, {50, 82}, {0, 1}, {0, 1});
}

void PineTree::initHitBoxes()
{
    collisionFunctionality->addHitBox("Trunk", sf::Vector2u(18, 5), sf::Vector2u(16, 76), scale);
}

PineTree::PineTree(const sf::Vector2f spawn_grid_position, sf::Texture &sprite_sheet, const float &scale)
    : Tree("Pine Tree", spawn_grid_position, sprite_sheet, scale)
{
    addSpriteLayer("Top");

    createAnimationFunctionality();
    createCollisionFunctionality();

    initAnimations();
    initHitBoxes();
}

PineTree::~PineTree() = default;

void PineTree::update(const float &dt, const sf::Vector2f &mouse_pos)
{
    animationFunctionality->play("CrownIdle");
    animationFunctionality->play("TrunkIdle");
    collisionFunctionality->update();
}

void PineTree::render(sf::RenderTarget &target)
{
    for (auto &[_, sprite] : layers)
    {
        if (sprite)
            target.draw(*sprite);
    }
}

void PineTree::render(sf::RenderTarget &target, const bool &show_hitboxes)
{
    for (auto &[_, sprite] : layers)
    {
        if (sprite)
            target.draw(*sprite);
    }

    if (show_hitboxes)
    {
        for (auto &[_, hitbox] : collisionFunctionality->getHitBoxes())
            target.draw(hitbox.rect);
    }
}
