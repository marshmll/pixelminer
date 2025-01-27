#include "Entities/Playable/Player.hxx"
#include "stdafx.hxx"

void Player::initPlayerAnimations()
{
    sf::Vector2u frame_size(16, 24);

    animationFunctionality->addAnimation("IdleDown", 10000, frame_size, {0, 0}, {0, 0});
    animationFunctionality->addAnimation("IdleUp", 10000, frame_size, {0, 1}, {0, 1});
    animationFunctionality->addAnimation("IdleLeft", 10000, frame_size, {0, 2}, {0, 2});
    animationFunctionality->addAnimation("IdleRight", 10000, frame_size, {0, 3}, {0, 3});
    animationFunctionality->addAnimation("WalkDown", 150, frame_size, {0, 0}, {3, 0});
    animationFunctionality->addAnimation("WalkUp", 150, frame_size, {0, 1}, {3, 1});
    animationFunctionality->addAnimation("WalkLeft", 150, frame_size, {0, 2}, {3, 2});
    animationFunctionality->addAnimation("WalkRight", 150, frame_size, {0, 3}, {3, 3});
}

Player::Player(const sf::Vector2f spawn_position, sf::Texture &sprite_sheet, const float &scale)
    : Entity("Player 1", spawn_position, sprite_sheet, scale)
{
    createMovementFunctionality(100.f, Movement::AllowAll);
    createAnimationFunctionality();
    createAttributeFunctionality(20, 20);

    initPlayerAnimations();

    std::cout << "[ Player ] -> Player \"" << name << "\" with id " << std::hex << id << " spawned at x: " << std::dec
              << spawn_position.x << ", y: " << spawn_position.y << "\n";
}

Player::~Player()
{
}

void Player::update(const float &dt, const bool &update_movement)
{
    if (update_movement)
    {
        movementFunctionality->update();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            move(dt, Up);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            move(dt, Down);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            move(dt, Left);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            move(dt, Right);
    }

    const MovementState mov_state = movementFunctionality->getState();
    const std::string mov_direction = movementFunctionality->getDirectionAsString();

    if (mov_state == MovementState::Idle)
        animationFunctionality->play("Idle" + mov_direction, true);

    else if (mov_state == MovementState::Walking)
        animationFunctionality->play("Walk" + mov_direction, true);
}

void Player::render(sf::RenderTarget &target)
{
    target.draw(sprite);
}