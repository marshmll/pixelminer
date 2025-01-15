#pragma once

#include "Engine/Configuration.hxx"
#include "Engine/GraphicsSettings.hxx"
#include "States/MainMenuState.hxx"
#include "Tiles/TileData.hxx"
#include "Tools/JSON.hxx"
#include "Tools/Logger.hxx"

class Engine
{
  private:
    /* ATTRIBUTES */
    Logger logger;
    GraphicsSettings gfx;

    sf::VideoMode vm;
    sf::RenderWindow window;
    std::optional<sf::Event> event;

    sf::Clock dtClock;
    float dt; // Delta time
    unsigned int gridSize;
    float scale;

    EngineData engineData;
    std::stack<std::unique_ptr<State>> states;

    // Assets
    std::map<std::string, sf::Font> fonts;
    std::map<std::string, sf::Texture> textures;
    std::map<std::uint32_t, TileData> tileData;
    std::map<std::string, sf::Shader> shaders;

    /* PRIVATE METHODS */

    void seedRandom();

    void initGraphicsSettings();

    void initVariables();

    void initTileData();

    void initFonts();

    void initTextures();

    void initShaders();

    void initEngineData();

    void initMainMenuState();

    void pollWindowEvents();

    void updateDeltaTime();

  public:
    Engine();
    virtual ~Engine();

    /* PUBLIC METHODS */

    void run();

    void update();

    void render();
};