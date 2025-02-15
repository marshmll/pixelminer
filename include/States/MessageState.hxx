#pragma once

#include "States/State.hxx"
#include "GUI/GUI.hxx"

class MessageState : public State
{
  private:
    sf::RectangleShape bg;

    std::unique_ptr<sf::Text> messageText;
    std::unique_ptr<sf::Text> descriptionText;
    std::unique_ptr<gui::TextButton> backButton;

  public:
    MessageState(EngineData &data, const std::string &message, const std::string &description);

    ~MessageState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};
