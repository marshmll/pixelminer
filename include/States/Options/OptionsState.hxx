/**
 * @file OptionsState.hxx
 * @brief Header file for the OptionsState class.
 */

#pragma once

#include "States/State.hxx"
#include "States/LocalesState.hxx"
#include "States/Options/VideoOptionsSubstate.hxx"
#include "GUI/GUI.hxx"

/**
 * @class OptionsState
 * @brief Represents the options menu state in the application.
 *
 * This class is responsible for managing the options menu, including rendering the background,
 * header, footer, and buttons for various options such as video settings, language selection,
 * and more. It also handles user interactions with the buttons.
 */
class OptionsState : public State
{
  private:
    sf::RectangleShape background; ///< Background shape for the options menu.
    sf::RectangleShape header;     ///< Header shape for the options menu.
    sf::RectangleShape footer;     ///< Footer shape for the options menu.

    sf::RectangleShape buttonContainer; ///< Container for the buttons in the options menu.
    std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons; ///< Map of buttons in the options menu.

    /**
     * @brief Initializes the GUI elements for the options menu.
     *
     * This function sets up the background, header, footer, and initializes the buttons.
     */
    void initGUI();

    /**
     * @brief Initializes the buttons for the options menu.
     *
     * This function creates and positions the buttons for various options such as video settings,
     * language selection, and more.
     */
    void initButtons();

  public:
    /**
     * @brief Constructs an OptionsState object.
     *
     * @param data Reference to the EngineData object containing necessary resources and settings.
     */
    OptionsState(EngineData &data);

    /**
     * @brief Destructor for the OptionsState object.
     */
    ~OptionsState();

    /**
     * @brief Updates the state of the options menu.
     *
     * This function handles user interactions with the buttons and updates the state accordingly.
     *
     * @param dt The delta time since the last update.
     */
    void update(const float &dt);

    /**
     * @brief Renders the options menu.
     *
     * This function draws the background, buttons, and other GUI elements to the render target.
     *
     * @param target The render target to draw the options menu on.
     */
    void render(sf::RenderTarget &target);
};
