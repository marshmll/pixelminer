#pragma once

#include "Entities/Inanimated/Trees/PineTree.hxx"
#include "Entities/Playable/Player.hxx"
#include "GUI/Chat.hxx"
#include "GUI/GUI.hxx"
#include "Map/EntitySpacialGridPartition.hxx"
#include "Map/Map.hxx"
#include "Map/TerrainGenerator.hxx"
#include "Network/Client.hxx"
#include "Network/Server.hxx"
#include "Player/PlayerGUI.hxx"
#include "States/State.hxx"
#include "Tools/UUID.hxx"

class GameState : public State
{
  private:
    std::unique_ptr<gui::PauseMenu> pauseMenu;

    std::vector<std::shared_ptr<Entity>> globalEntities; // All entities apart from players

    std::unordered_map<std::string, std::shared_ptr<Player>> players; // Map UUID to player
    std::shared_ptr<Player> thisPlayer;

    std::unique_ptr<EntitySpacialGridPartition> entitySpacialGridPartition;
    std::queue<Cell *> spatialGridCellBuffer;

    sf::View playerCamera;
    std::unique_ptr<PlayerGUI> playerGUI;

    std::unique_ptr<Map> map;

    sf::RectangleShape loadingBg;
    std::unique_ptr<sf::Text> loadingText;
    std::unique_ptr<sf::Text> loadingMsg;

    std::unique_ptr<sf::Text> debugText;

    bool debugInfo;
    bool debugChunks;
    bool debugHitBoxes;

    Client client;
    Server server;

    std::unique_ptr<gui::Chat> chat;

    void initLoadingScreen();

    void initMap();

    void initMap(const std::string &map_folder_name);

    void initEntitySpatialGridPartition();

    void initThisPlayer();

    void initPlayerGUI();

    void initPlayerCamera();

    void initPauseMenu();

    void initChat();

    void initDebugging();

    void resolveCollision(std::shared_ptr<Entity> first_entity, std::shared_ptr<Entity> second_entity,
                          const sf::FloatRect &intersection);

  public:
    GameState(EngineData &data);

    GameState(EngineData &data, const std::string &map_folder_name);

    ~GameState();

    void update(const float &dt);

    void updatePauseMenu(const float &dt);

    void updateMap(const float &dt);

    void updateGlobalEntities(const float &dt);

    void updatePlayers(const float &dt);

    void updateCollisions(const float &dt);

    void updatePlayerCamera();

    void updateChat(const float &dt);

    void updateDebugText(const float &dt);

    void render(sf::RenderTarget &target);

    void renderGlobalEntities(sf::RenderTarget &target);

    void saveWorld();
};