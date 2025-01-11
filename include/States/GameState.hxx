#pragma once

#include "Entities/Playable/Player.hxx"
#include "GUI/Utils.hxx"
#include "Map/Map.hxx"
#include "States/State.hxx"

class GameState : public State
{
  private:
    std::map<std::pair<sf::IpAddress, unsigned short>, std::shared_ptr<Player>> players;
    std::shared_ptr<Player> thisPlayer;

    sf::View playerCamera;

    std::unique_ptr<Map> map;

    // MULTIPLAYER
    sf::UdpSocket udpSocket;
    sf::SocketSelector socketSelector;

    std::unique_ptr<sf::Text> debugText;

    void initMap();

    void initThisPlayer();

    void initPlayerCamera();

    void initUdpSocket();

    void initNetworkThreads(const unsigned short port);

    void initDebugging();

    void broadcastGameStateThread();

    void receiveGameStateThread();

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