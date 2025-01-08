#include "States/GameState.hxx"
#include "stdafx.hxx"

void GameState::initThisPlayer()
{
    players["ThisPlayer"] =
        std::make_unique<Player>(sf::Vector2f(100.f, 100.f), data.textures->at("Player1"), data.scale);
}

GameState::GameState(StateData &data) : State(data)
{
    initThisPlayer();
    // map = std::make_unique<Map>(sf::Vector3<unsigned int>(500, 500, 1));
}

GameState::~GameState()
{
}

void GameState::update(const float &dt)
{
    for (auto &[name, player] : players)
        player->update(dt);
}

void GameState::render(sf::RenderTarget &target)
{
    for (auto &[name, player] : players)
        player->render(target);
}