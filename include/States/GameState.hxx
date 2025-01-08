#pragma once

#include "Entities/Playable/Player.hxx"
#include "GUI/Utils.hxx"
#include "Map/Map.hxx"
#include "States/State.hxx"

class GameState : public State
{
  private:
    std::unique_ptr<Map> map;
    std::unique_ptr<sf::Text> debugText;
    std::map<std::pair<sf::IpAddress, unsigned short>, std::unique_ptr<Player>> players;

    // MULTIPLAYER
    sf::UdpSocket udpSocket;
    sf::SocketSelector socketSelector;

    void initThisPlayer();

    void initMap();

    void initUdpSocket();

    void initNetworkThreads(const unsigned short port);

    void initDebugging();

    void broadcastGameStateThread();

    void receiveGameStateThread();

  public:
    GameState(StateData &data);
    ~GameState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};