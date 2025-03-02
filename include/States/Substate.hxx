#pragma once

#include "Engine/EngineData.hxx"

class Substate
{
  protected:
    EngineData &data; ///< Reference to the engine data.

    sf::Vector2i mousePosScreen; ///< Mouse position in screen coordinates.
    sf::Vector2i mousePosWindow; ///< Mouse position in window coordinates.
    sf::Vector2f mousePosView;   ///< Mouse position in view coordinates.
    sf::Vector2i mousePosGrid;   ///< Mouse position in grid coordinates.

    bool dead;       ///< Flag indicating if the substate is marked as dead (should be removed).
    bool restartAll; ///< Flag to tell engine to restart all states in the stack;

    sf::Clock keyClock;   ///< Clock used to track time for key press events.
    sf::Clock mouseClock; ///< Clock used to track time for mouse button press events.

  public:
    Substate(EngineData &data);

    virtual ~Substate();

    /**
     * @brief Updates the current substate, including logic and input handling.
     * @param dt The delta time (time elapsed since the last update).
     */
    virtual void update(const float &dt) = 0;

    /**
     * @brief Renders the current substate to the provided render target.
     * @param target The target render object to draw the substate elements.
     */
    virtual void render(sf::RenderTarget &target) = 0;

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
     * @brief Marks the current substate as dead (should be removed).
     */
    virtual void killSelf();

    /**
     * @brief Triggers the engine to pop all states in the stack and push a new `MainMenuState`.
     */
    void restartStates();

    /**
     * @brief Checks if the current state is dead.
     * @return True if the state is dead, false otherwise.
     */
    const bool &isDead() const;

    /**
     * @brief Checks if the current state has asked to restart all states in the engine stack.
     * @return True if the restart flag is on, false otherwise.
     */
    const bool &askedToRestartAllStates() const;
};
