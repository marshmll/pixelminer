/**
 * @file AddServerState.hxx
 * @brief Declares the AddServerState class responsible for adding a server in the servers.json file.
 */

#pragma once

#include "GUI/GUI.hxx"
#include "States/State.hxx"

/**
 * @class AddServerState
 * @brief Manages the UI and logic for adding a new server to the servers.json file.
 */
class AddServerState : public State
{
  private:
    sf::RectangleShape background;            ///< Background shape for the UI.
    std::unique_ptr<gui::Input> nameInput;    ///< Input field for the server name.
    std::unique_ptr<gui::Input> addressInput; ///< Input field for the server address.

    std::unordered_map<std::string, std::unique_ptr<gui::TextButton>> buttons; ///< Buttons for UI interaction.

    /**
     * @brief Initializes the graphical user interface components.
     */
    void initGUI();

    /**
     * @brief Adds a new server entry to the servers.json file.
     * @return True if the server was successfully added, false otherwise.
     */
    const bool addServer();

  public:
    /**
     * @brief Constructs an AddServerState object.
     * @param data Reference to the game engine data.
     */
    AddServerState(EngineData &data);

    /**
     * @brief Destructor for AddServerState.
     */
    ~AddServerState();

    /**
     * @brief Updates the state logic.
     * @param dt Time delta for smooth updates.
     */
    void update(const float &dt);

    /**
     * @brief Renders the UI components on the target.
     * @param target Render target.
     */
    void render(sf::RenderTarget &target);
};
