#include "States/GameState.hxx"
#include "stdafx.hxx"

GameState::GameState(StateData &data) : State(data)
{
    entity = std::make_unique<Entity>("Entity", sf::Vector2f(100.f, 100.f));
}

GameState::~GameState()
{
}

void GameState::update(const float &dt)
{
    entity->update(dt);
}

void GameState::render(sf::RenderTarget &target)
{
    entity->render(target);
}