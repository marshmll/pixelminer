#pragma once

#include "Entities/Playable/Player.hxx"
#include "Map/Map.hxx"
#include "States/State.hxx"

class GameState : public State
{
  private:
    std::unique_ptr<Map> map;
    std::map<std::string, std::unique_ptr<Player>> players;

    void initThisPlayer();

  public:
    GameState(StateData &data);
    ~GameState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};