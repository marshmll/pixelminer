/**
 * @file Engine.hxx
 * @brief Defines the Engine class for managing game execution.
 */

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

/**
 * @class Engine
 * @brief Manages the core game engine functionalities.
 */
class Engine
{
  private:
    std::string myUuid;   ///< Unique identifier for the engine instance.
    Logger logger;        ///< Logger instance for logging messages.
    GraphicsSettings gfx; ///< Graphics settings configuration.

    sf::VideoMode vm;               ///< Video mode settings.
    sf::RenderWindow window;        ///< Main game window.
    std::optional<sf::Event> event; ///< Event handling.

    float dt;              ///< Delta time for frame updates.
    sf::Clock dtClock;     ///< Clock to measure delta time.
    unsigned int gridSize; ///< Size of the grid.
    unsigned int scale;    ///< Scale factor for rendering.

    EngineData engineData;                     ///< Engine-related data storage.
    std::stack<std::shared_ptr<State>> states; ///< Stack of game states.

    std::unordered_map<std::string, ResourcePack> resourcePacks; ///< Available resource packs.
    std::shared_ptr<ResourcePack> activeResourcePack;            ///< Currently active resource pack.

    /**
     * @brief Seeds the random number generator.
     */
    void seedRandom();

    /**
     * @brief Verifies the existence of the global folder and creates it if necessary.
     */
    void verifyGlobalFolder();

    /**
     * @brief Identifies the engine instance by generating or retrieving a UUID.
     */
    void identificateSelf();

    /**
     * @brief Initializes graphics settings from a configuration file.
     */
    void initGraphicsSettings();

    /**
     * @brief Initializes engine-related variables.
     */
    void initVariables();

    /**
     * @brief Loads available resource packs.
     */
    void initResourcePacks();

    /**
     * @brief Initializes engine data structures.
     */
    void initEngineData();

    /**
     * @brief Initializes the main menu state.
     */
    void initMainMenuState();

    /**
     * @brief Polls window events and processes them.
     */
    void pollWindowEvents();

    /**
     * @brief Updates the delta time.
     */
    void updateDeltaTime();

    /**
     * @brief Updates the engine state.
     */
    void update();

    /**
     * @brief Renders the game state.
     */
    void render();

  public:
    /**
     * @brief Constructs an Engine instance.
     */
    Engine();

    /**
     * @brief Destructor for the Engine class.
     */
    virtual ~Engine();

    /**
     * @brief Runs the game loop.
     */
    void run();
};
