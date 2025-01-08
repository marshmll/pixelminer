#include "Entities/Playable/Player.hxx"
#include "stdafx.hxx"

Player::Player(const sf::Vector2f spawn_position, sf::Texture &sprite_sheet, const float &scale)
    : Entity("Player", spawn_position, sprite_sheet, scale)
{
    createMovementFunctionality(100.f, Movement::AllowAll);

    sprite.setTextureRect(sf::IntRect({0, 0}, {16, 24}));

    std::cout << "[ Player ] -> Player with id " << std::hex << id << " spawned at (" << std::dec << spawn_position.x
              << ", " << spawn_position.y << ")"
              << "\n";
}

Player::~Player()
{
}

void Player::update(const float &dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        move(dt, Up);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        move(dt, Down);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        move(dt, Left);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        move(dt, Right);
}

void Player::render(sf::RenderTarget &target)
{
    target.draw(sprite);
}