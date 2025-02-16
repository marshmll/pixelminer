/**
 * @file MessageState.hxx
 * @brief Header file for the `MessageState` class, which represents a state for displaying messages in the application.
 * This state is used to show a message and a description, along with a back button to return to the previous state.
 */

#pragma once

#include "States/State.hxx"
#include "GUI/GUI.hxx"

/**
 * @class MessageState
 * @brief A state that displays a message, a description, and a back button.
 *
 * This class inherits from the `State` class and is responsible for rendering a message screen.
 * It includes a background, a message text, a description text, and a back button to exit the state.
 */
class MessageState : public State
{
  private:
    sf::RectangleShape bg; ///< Background rectangle for the message state.

    std::unique_ptr<sf::Text> messageText;       ///< Text object for the main message.
    std::unique_ptr<sf::Text> descriptionText;   ///< Text object for the description.
    std::unique_ptr<gui::TextButton> backButton; ///< Button to return to the previous state.

  public:
    /**
     * @brief Constructs a `MessageState` object.
     *
     * @param data Reference to the `EngineData` object containing shared resources.
     * @param message The main message to display.
     * @param description The description text to display below the message.
     */
    MessageState(EngineData &data, const std::string &message, const std::string &description);

    /**
     * @brief Destructor for the `MessageState` object.
     */
    ~MessageState();

    /**
     * @brief Updates the state logic.
     *
     * This function is called every frame to update the state's logic, such as button interactions.
     *
     * @param dt The time elapsed since the last frame, in seconds.
     */
    void update(const float &dt);

    /**
     * @brief Renders the state to the target.
     *
     * This function draws the background, message text, description text, and back button to the render target.
     *
     * @param target The render target to draw the state onto.
     */
    void render(sf::RenderTarget &target);
};