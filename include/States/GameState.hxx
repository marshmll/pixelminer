#pragma once

#include "Entities/Entity.hxx"
#include "Map/Map.hxx"
#include "States/State.hxx"

class GameState : public State
{
  private:
    std::unique_ptr<Map> map;
    std::unique_ptr<Entity> entity;

  public:
    GameState(StateData &data);
    ~GameState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};