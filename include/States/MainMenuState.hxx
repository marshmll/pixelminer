/**
 * @file MainMenuState.hxx
 * @brief Declares the MainMenuState class to manage the principal screen of the game, allowing
 * configuration changes and game start
 */

#pragma once

#include "GUI/GUI.hxx"
#include "States/GameState.hxx"
#include "States/MultiplayerState.hxx"
#include "States/WorldSelectionMenuState.hxx"

/**
 * @class MainMenuState
 * @brief Represents the main menu state of the game, providing options such as starting a singleplayer game,
 * multiplayer game, accessing options, and quitting the game.
 *
 * The MainMenuState class handles the GUI for the main menu of the game, including buttons for navigating to different
 * game states such as singleplayer, multiplayer, and options. It also manages the background and title display for the
 * menu.
 */
class MainMenuState : public State
{
  private:
    sf::RectangleShape bgShape;       ///< Background shape of the main menu.
    std::unique_ptr<sf::Text> title;  ///< Title text displayed at the top of the main menu.
    std::unique_ptr<sf::Text> shadow; ///< Title text shadow displayed at the top of the main menu.

    std::map<std::string, std::unique_ptr<gui::Button>>
        buttons; ///< Map containing the buttons in the main menu, such as Singleplayer, Multiplayer, Options, and Quit.

    /**
     * @brief Initializes the graphical user interface (GUI) elements for the main menu.
     *
     * This method sets up the background, title, and button positions and styles.
     */
    void initGUI();

  public:
    /**
     * @brief Constructs the MainMenuState object.
     * @param data The engine data containing resources, window, and other necessary information.
     */
    MainMenuState(EngineData &data);

    /**
     * @brief Destructor for cleaning up resources used by the main menu state.
     */
    ~MainMenuState();

    /**
     * @brief Updates the main menu state, including button interactions and input handling.
     * @param dt The delta time (time elapsed since the last update).
     */
    void update(const float &dt);

    /**
     * @brief Renders the main menu state, including the background, title, and buttons.
     * @param target The target render object to draw the menu elements.
     */
    void render(sf::RenderTarget &target);
};
