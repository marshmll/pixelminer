/**
 * @file GameState.hxx
 * @brief Declares the GameState class to manage the core logic of a game instance.
 */

#pragma once

#include "Animations/Animation.hxx"
#include "Entities/EntityRenderPriorityQueue.hxx"
#include "Entities/Inanimated/Trees/PineTree.hxx"
#include "Entities/Playable/Player.hxx"
#include "GUI/Chat.hxx"
#include "GUI/GUI.hxx"
#include "Game/Commands/CommandInterpreter.hxx"
#include "Game/GameContext.hxx"
#include "Map/EntitySpatialGridPartition.hxx"
#include "Map/Map.hxx"
#include "Map/TerrainGenerator.hxx"
#include "Network/Client.hxx"
#include "Network/Server.hxx"
#include "Player/PlayerGUI.hxx"
#include "States/State.hxx"
#include "Tools/UUID.hxx"
#include "Engine/Languages.hxx"

/**
 * @class GameState
 * @brief The GameState class manages the core logic of the game including players, entities, collisions, and rendering.
 *
 * This class handles game updates, rendering, player interactions, collisions, map loading, and other in-game
 * processes. It is responsible for managing global entities and players, interacting with networking components, and
 * providing the user interface and debugging features.
 */
class GameState : public State
{
  private:
    GameContext ctx;

    std::unique_ptr<CommandInterpreter> cmd;

    std::unique_ptr<gui::PauseMenu> pauseMenu; ///< Pointer to the pause menu GUI.

    std::shared_ptr<Player> thisPlayer; ///< Pointer to the current player.

    std::unique_ptr<EntitySpatialGridPartition>
        entitySpacialGridPartition; ///< Partition grid for entities' spatial organization

    EntityRenderPriorityQueue
        entityRenderPriorityQueue; ///< A priority queue that manages the entity's order of rendering.

    sf::View playerCamera; ///< Camera view for the player.

    std::unique_ptr<PlayerGUI> playerGUI; ///< Player's GUI interface.

    sf::Clock miningClock;
    sf::Vector2i lastHoveredTile;

    sf::RectangleShape loadingBg;          ///< Background rectangle for the loading screen.
    std::unique_ptr<sf::Text> loadingText; ///< Text for loading status.
    std::unique_ptr<sf::Text> loadingMsg;  ///< Message text for loading status.

    std::unique_ptr<sf::Text> debugText; ///< Text for debugging information.

    bool debugInfo;     ///< Flag to toggle displaying debug information.
    bool debugChunks;   ///< Flag to toggle displaying debug chunk data.
    bool debugHitBoxes; ///< Flag to toggle displaying hitboxes for entities.

    std::unique_ptr<sf::Sprite> loaderSprite;   ///< The sprite of the loader animation.
    std::unique_ptr<Animation> loaderAnimation; ///< The loader animation.

    std::unique_ptr<gui::Chat> chat; ///< Chat interface.

    Server server; ///< Server component for multiplayer gamess

    /**
     * @brief Initializes the loading screen.
     */
    void initLoadingScreen();

    /**
     * @brief Initializes the map for the game.
     */
    void initMap();

    /**
     * @brief Initializes the map for the game using a specific folder name.
     * @param map_folder_name The folder containing map data.
     */
    void initMap(const std::string &map_folder_name);

    /**
     * @brief Initializes the entity spatial grid partition.
     */
    void initEntitySpatialGridPartition();

    /**
     * @brief Initializes the player character.
     */
    void initThisPlayer();

    /**
     * @brief Initializes the player's GUI.
     */
    void initPlayerGUI();

    /**
     * @brief Initializes the mining's clock.
     */
    void initMiningClock();

    /**
     * @brief Initializes the camera view for the player.
     */
    void initPlayerCamera();

    /**
     * @brief Initializes the chat interface.
     */
    void initChat();

    /**
     * @brief Initializes the pause menu.
     */
    void initPauseMenu();

    /**
     * @brief Initializes the game command interpreter;
     */
    void initCommandInterpreter();

    /**
     * @brief Initializes debugging settings and displays.
     */
    void initDebugging();

    /**
     * @brief Resolves the collision between two entities.
     * @param first_entity The first entity involved in the collision.
     * @param second_entity The second entity involved in the collision.
     * @param intersection The intersection rectangle of the collision.
     */
    void resolveCollision(std::shared_ptr<Entity> &first_entity, std::shared_ptr<Entity> &second_entity,
                          const sf::FloatRect &intersection);

    void handleTileMining();

  public:
    /**
     * @brief Constructor that initializes the game state with given engine data.
     * @param data The engine data containing resources and settings.
     */
    GameState(EngineData &data);

    /**
     * @brief Constructor that initializes the game state with a specific map folder name.
     * @param data The engine data containing resources and settings.
     * @param map_folder_name The folder containing map data.
     */
    GameState(EngineData &data, const std::string &map_folder_name);

    /**
     * @brief Destructor to clean up the game state.
     */
    ~GameState();

    /**
     * @brief Updates the game state.
     * @param dt The delta time for the frame update.
     */
    void update(const float &dt);

    /**
     * @brief Updates the pause menu.
     * @param dt The delta time for the frame update.
     */
    void updatePauseMenu(const float &dt);

    /**
     * @brief Updates the map elements.
     * @param dt The delta time for the frame update.
     */
    void updateMap(const float &dt);

    /**
     * @brief Updates global entities in the game world.
     * @param dt The delta time for the frame update.
     */
    void updateGlobalEntities(const float &dt);

    /**
     * @brief Updates all players in the game.
     * @param dt The delta time for the frame update.
     */
    void updatePlayers(const float &dt);

    /**
     * @brief Updates the order of the entities to be rendered in the render queue.
     */
    void updateEntityRenderPriorityQueue();

    /**
     * @brief Updates collisions between entities.
     * @param dt The delta time for the frame update.
     */
    void updateCollisions(const float &dt);

    /**
     * @brief Updates the camera view for the player.
     */
    void updatePlayerCamera();

    /**
     * @brief Updates the chat interface.
     * @param dt The delta time for the frame update.
     */
    void updateChat(const float &dt);

    /**
     * @brief Updates the debug text information.
     * @param dt The delta time for the frame update.
     */
    void updateDebugText(const float &dt);

    /**
     * @brief Renders the game state to the target render target.
     * @param target The render target to draw the game state to.
     */
    void render(sf::RenderTarget &target);

    /**
     * @brief Renders global entities in the game world from the entity render priority queue.
     * @param target The render target to draw the entities to.
     */
    void renderGlobalEntities(sf::RenderTarget &target);

    /**
     * @brief Saves the current world state, including the map and players.
     */
    void saveWorld();
};
