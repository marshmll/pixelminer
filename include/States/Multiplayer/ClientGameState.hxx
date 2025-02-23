/**
 * @file ClientGameState.hxx
 * @brief Declares the ClientGameState for client players to play in a remote server.
 */

#pragma once

#include "States/MessageState.hxx"
#include "States/State.hxx"
#include "Network/Client.hxx"
#include "GUI/GUI.hxx"
#include "Animations/Animation.hxx"

/**
 * @class ClientGameState
 * @brief Implements a game state specific for client players for playing in a remote server's world.
 */
class ClientGameState : public State
{
  private:
    Client client; ///< Client networking component for ClientGameState.

    sf::RectangleShape feedbackBg;          ///< A background for the connection feedback screen.
    std::unique_ptr<sf::Text> feedbackText; ///< A text for the connection feedback screen.
    std::unique_ptr<sf::Text> feedbackMsg;  ///< A message for the connection feedback screen.

    std::unique_ptr<sf::Sprite> loaderSprite;   ///< A sprite for the loader.
    std::unique_ptr<Animation> loaderAnimation; ///< A animation for the loader.

    bool ready; ///< A flag that indicates if the state has a result.

    /**
     * @brief Initializes the feedback screen for the state.
     */
    void initFeedbackScreen();

  public:
    /**
     * @brief Constructor for the ClientGameState class.
     * @param data The engine data reference.
     * @param ip The ip of the server to connect to.
     * @param port The port of the server to connect to.
     */
    ClientGameState(EngineData &data, const sf::IpAddress &ip, const unsigned short &port);

    /**
     * @brief Destructor for the ClientGameState class.
     */
    ~ClientGameState();

    /**
     * @brief Updates the state.
     * @param dt The delta time since last rendered frame.
     */
    void update(const float &dt);

    /**
     * @brief Renders the state into a target.
     * @param target A target to render the state to.
     */
    void render(sf::RenderTarget &target);

    /**
     * @brief Updates the feedback screen.
     * @param dt The delta time since last rendered frame.
     */
    void updateFeedbackScreen(const float &dt);

    /**
     * @brief Renders the feedback screen into a target..
     * @param target A target to render the feedback screen to.
     */
    void renderFeedbackScreen(sf::RenderTarget &target);
};
