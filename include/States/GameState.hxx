#pragma once

#include "Entities/Playable/Player.hxx"
#include "GUI/Utils.hxx"
#include "Map/Map.hxx"
#include "Network/Client.hxx"
#include "Network/Server.hxx"
#include "States/State.hxx"

class GameState : public State
{
  private:
    std::map<std::pair<sf::IpAddress, unsigned short>, std::shared_ptr<Player>> players;
    std::shared_ptr<Player> thisPlayer;

    sf::View playerCamera;

    std::unique_ptr<Map> map;

    Server server;
    Client client;

    std::unique_ptr<sf::Text> debugText;

    void initServer();

    void initMap();

    void initThisPlayer();

    void initPlayerCamera();

    void initDebugging();

  public:
    GameState(StateData &data);

    ~GameState();

    void update(const float &dt);

    void updateMap(const float &dt);

    void updatePlayers(const float &dt);

    void updatePlayerCamera();

    void updateDebugText(const float &dt);

    void render(sf::RenderTarget &target);
};