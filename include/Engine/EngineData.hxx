#pragma once

#include "Engine/ResourcePack.hxx"

class State;

/**
 * @struct EngineData
 * @brief Contains data necessary for the game engine, such as window, resource packs, and current state.
 *
 * This structure holds various data related to the game engine, such as the unique identifier (`uuid`), window
 * configuration, the stack of states, active resource pack, and mouse event data.
 */
struct EngineData
{
    std::string *currentLocale;                 ///< The current locale. Used to select game translation.
    std::string uuid;                           ///< Unique identifier for the engine instance.
    unsigned int gridSize;                      ///< Size of the grid used for rendering.
    unsigned int scale;                         ///< Scale factor for rendering.
    std::stack<std::shared_ptr<State>> *states; ///< Stack of the current states in the game.
    std::unordered_map<std::string, ResourcePack> *resourcePacks; ///< Map of resource packs available in the game.
    std::shared_ptr<ResourcePack> activeResourcePack;             ///< The currently active resource pack.
    sf::RenderWindow *window;                                     ///< Pointer to the window being used for rendering.
    sf::VideoMode *vm;                                            ///< Pointer to the current video mode (resolution).
    std::optional<sf::Event> *event;                              ///< Pointer to the current event.
};
