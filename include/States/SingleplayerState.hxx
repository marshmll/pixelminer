#pragma once

#include "GUI/GUI.hxx"
#include "States/State.hxx"

class SingleplayerState : public State
{
  private:
    sf::RectangleShape background;
    sf::RectangleShape header;
    sf::RectangleShape footer;

    sf::RectangleShape buttonContainer;

    std::map<std::string, std::unique_ptr<gui::TextButton>> buttons;

    void initGUI();

  public:
    SingleplayerState(EngineData &data);

    ~SingleplayerState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);

    void updateGUI(const float &dt);

    void renderGUI(sf::RenderTarget &target);
};