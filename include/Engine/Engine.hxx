#pragma once

#include "Engine/Configuration.hxx"
#include "Engine/GraphicsSettings.hxx"
#include "Engine/ResourcePack.hxx"
#include "States/MainMenuState.hxx"
#include "Tiles/TileData.hxx"
#include "Tools/JSON.hxx"
#include "Tools/Logger.hxx"
#include "Tools/UUID.hxx"

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

    sf::Clock dtClock;
    float dt; // Delta time
    unsigned int gridSize;
    unsigned int scale;

    EngineData engineData;
    std::stack<std::unique_ptr<State>> states;

    std::map<std::string, ResourcePack> resourcePacks;
    std::shared_ptr<ResourcePack> activeResourcePack;

    /* PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    void seedRandom();

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