#pragma once

#include "States/State.hxx"
#include "GUI/GUI.hxx"

class OptionsState : public State
{
  private:
    sf::RectangleShape background;
    sf::RectangleShape header;
    sf::RectangleShape footer;

    sf::RectangleShape buttonContainer;
    std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons;

    void initGUI();

  public:
    OptionsState(EngineData &data);

    ~OptionsState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};
