/**
 * @file State.hxx
 * @brief Declares the State class to hold all necessary data to a generic engine state.
 *
 * A engine state represents any individual "screen" or "section" that is responsible for
 * well-defined functionalities and actions.
 */

#pragma once

#include "Engine/Configuration.hxx"
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
    std::string uuid;                                             ///< Unique identifier for the engine instance.
    unsigned int gridSize;                                        ///< Size of the grid used for rendering.
    unsigned int scale;                                           ///< Scale factor for rendering.
    std::stack<std::shared_ptr<State>> *states;                   ///< Stack of the current states in the game.
    std::unordered_map<std::string, ResourcePack> *resourcePacks; ///< Map of resource packs available in the game.
    std::shared_ptr<ResourcePack> activeResourcePack;             ///< The currently active resource pack.
    sf::RenderWindow *window;                                     ///< Pointer to the window being used for rendering.
    sf::VideoMode *vm;                                            ///< Pointer to the current video mode (resolution).
    std::optional<sf::Event> *event;                              ///< Pointer to the current event.
    sf::Event::MouseWheelScrolled
        mouseData; ///< Data for mouse wheel scroll events (access after checking the event type).
};

/**
 * @class State
 * @brief Abstract base class for game states.
 *
 * The State class represents a game state in the engine. It provides functionality to handle state updates, rendering,
 * and user input (keyboard and mouse). Specific game states, such as menus or gameplay states, should derive from this
 * class and implement their own behavior.
 */
class State
{
  private:
  protected:
    EngineData &data; ///< Reference to the engine data.

    sf::RenderTexture renderTexture; ///< Texture used for rendering the current state.
    sf::Sprite renderSprite;         ///< Sprite used to display the render texture.

    sf::Vector2i mousePosScreen; ///< Mouse position in screen coordinates.
    sf::Vector2i mousePosWindow; ///< Mouse position in window coordinates.
    sf::Vector2f mousePosView;   ///< Mouse position in view coordinates.
    sf::Vector2i mousePosGrid;   ///< Mouse position in grid coordinates.

    bool dead;     ///< Flag indicating if the state is marked as dead (should be removed).
    bool replaced; ///< Flag indicating if the state has been replaced by another state.

    sf::Clock keyClock;   ///< Clock used to track time for key press events.
    sf::Clock mouseClock; ///< Clock used to track time for mouse button press events.

    std::shared_ptr<State> replacerState; ///< State that will replace the current state.

  public:
    /**
     * @brief Constructs a new State object.
     * @param data The engine data containing resources, window, and other necessary information.
     */
    State(EngineData &data);

    /**
     * @brief Destructor for cleaning up resources used by the state.
     */
    virtual ~State();

    /**
     * @brief Updates the current state, including logic and input handling.
     * @param dt The delta time (time elapsed since the last update).
     */
    virtual void update(const float &dt);

    /**
     * @brief Renders the current state to the provided render target.
     * @param target The target render object to draw the state elements.
     */
    virtual void render(sf::RenderTarget &target);

    /**
     * @brief Updates the mouse position based on the window and view.
     * @param view The view to consider for mapping the mouse position, or empty to use the default view.
     */
    void updateMousePositions(std::optional<sf::View> view = {});

    /**
     * @brief Checks if a specific key has been pressed within a given time period.
     * @param milliseconds The time period in milliseconds within which the key must be pressed.
     * @param key The keyboard key to check.
     * @return True if the key has been pressed within the specified time period, false otherwise.
     */
    const bool keyPressedWithin(const std::int32_t &milliseconds, const sf::Keyboard::Key &key);

    /**
     * @brief Checks if a specific mouse button has been pressed within a given time period.
     * @param milliseconds The time period in milliseconds within which the button must be pressed.
     * @param button The mouse button to check.
     * @return True if the button has been pressed within the specified time period, false otherwise.
     */
    const bool mouseButtonPressedWithin(const std::int32_t &milliseconds, const sf::Mouse::Button &button);

    /**
     * @brief Marks the current state as dead (should be removed).
     */
    virtual void killSelf();

    /**
     * @brief Replaces the current state with another state in the stack.
     * @param state The state that will replace the current state.
     */
    void replaceSelf(std::shared_ptr<State> state);

    /**
     * @brief Gets the state that will replace the current state.
     * @return A reference to the replacer state.
     */
    std::shared_ptr<State> &getReplacerState();

    /**
     * @brief Checks if the current state is dead.
     * @return True if the state is dead, false otherwise.
     */
    const bool &isDead() const;

    /**
     * @brief Checks if the current state has been replaced.
     * @return True if the state has been replaced, false otherwise.
     */
    const bool &wasReplaced() const;
};
