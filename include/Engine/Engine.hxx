#pragma once

#include "Engine/Configuration.hxx"
#include "Engine/GraphicsSettings.hxx"
#include "Engine/ResourcePack.hxx"
#include "States/MainMenuState.hxx"
#include "Tiles/TileData.hxx"
#include "Tools/JSON.hxx"
#include "Tools/Logger.hxx"
#include "Tools/UUID.hxx"
#include "Tools/Zip.hxx"

class Engine
{
  private:
    /* ATTRIBUTES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    std::string myUuid;
    Logger logger;
    GraphicsSettings gfx;

    sf::VideoMode vm;
    sf::RenderWindow window;
    std::optional<sf::Event> event;

    float dt; // Delta time
    sf::Clock dtClock;
    unsigned int gridSize;
    unsigned int scale;

    EngineData engineData;
    std::stack<std::shared_ptr<State>> states;

    std::unordered_map<std::string, ResourcePack> resourcePacks;
    std::shared_ptr<ResourcePack> activeResourcePack;

    /* PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    void seedRandom();

    void verifyGlobalFolder();

    void identificateSelf();

    void initGraphicsSettings();

    void initVariables();

    void initResourcePacks();

    void initEngineData();

    void initMainMenuState();

    void pollWindowEvents();

    void updateDeltaTime();

    void update();

    void render();

  public:
    Engine();

    virtual ~Engine();

    /* PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    void run();
};