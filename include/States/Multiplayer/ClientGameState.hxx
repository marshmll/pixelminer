#pragma once

#include "States/MessageState.hxx"
#include "States/State.hxx"
#include "Network/Client.hxx"
#include "GUI/GUI.hxx"
#include "Animations/Animation.hxx"

class ClientGameState : public State
{
  private:
    Client client;

    sf::RectangleShape feedbackBg;
    std::unique_ptr<sf::Text> feedbackText;
    std::unique_ptr<sf::Text> feedbackMsg;

    std::unique_ptr<sf::Sprite> loaderSprite;
    std::unique_ptr<Animation> loaderAnimation;

    bool ready;

    void initFeedbackScreen();

  public:
    ClientGameState(EngineData &data, const sf::IpAddress &ip, const unsigned short &port);

    ~ClientGameState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);

    void updateFeedbackScreen(const float &dt);

    void renderFeedbackScreen(sf::RenderTarget &target);
};
