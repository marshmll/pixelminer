/**
 * @file DirectConnectState.hxx
 * @brief Declares the DirectConnectState class to manage direct server connections.
 */

#pragma once

#include "GUI/GUI.hxx"
#include "States/State.hxx"

/**
 * @class DirectConnectState
 * @brief A state that allows the player to directly connect to a server by entering an address.
 *
 * The `DirectConnectState` class represents a game state in which the player can input a server
 * address and choose to either join the server or cancel the connection. It provides a graphical
 * interface with an address input field and buttons for joining or canceling the connection.
 *
 * This class manages the interaction with the user through GUI elements, updating the state based
 * on user input, and rendering the interface to the screen.
 */
class DirectConnectState : public State
{
  private:
    /**
     * @brief A rectangle shape representing the background of the state.
     */
    sf::RectangleShape background;

    /**
     * @brief A unique pointer to the input field for entering the server address.
     */
    std::unique_ptr<gui::Input> addrInput;

    /**
     * @brief A map of buttons that the user can interact with in this state.
     */
    std::map<std::string, std::unique_ptr<gui::TextButton>> buttons;

    /**
     * @brief Initializes the graphical user interface (GUI) elements for the DirectConnectState.
     *
     * This method sets up the background, input fields, and buttons for the state, such as the "Join Server"
     * and "Cancel" buttons, and configures their appearance and position.
     */
    void initGUI();

  public:
    /**
     * @brief Constructs a DirectConnectState instance.
     *
     * @param data A reference to the engine data containing relevant resources and information
     * about the current game state.
     */
    DirectConnectState(EngineData &data);

    /**
     * @brief Destroys the DirectConnectState instance.
     *
     * The destructor cleans up any resources related to the state, such as GUI components.
     */
    ~DirectConnectState();

    /**
     * @brief Updates the state of the DirectConnectState.
     *
     * This method updates the mouse position, handles user input for the address input field, updates
     * the state of buttons, and checks for button presses such as the "Cancel" button.
     *
     * @param dt The delta time since the last update.
     */
    void update(const float &dt);

    /**
     * @brief Renders the DirectConnectState to the screen.
     *
     * This method renders the background, address input field, and buttons of the DirectConnectState
     * onto the specified render target.
     *
     * @param target The render target to draw the state elements on.
     */
    void render(sf::RenderTarget &target);
};
