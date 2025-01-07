#pragma once

#include "GUI/GUI.hxx"
#include "States/State.hxx"

class MainMenuState : public State
{
  private:
    sf::Texture bgTexture;
    sf::RectangleShape bgShape;
    std::unique_ptr<sf::Text> title;

    std::map<std::string, std::unique_ptr<GUI::Button>> buttons;

    void initGUI();

  public:
    MainMenuState(StateData &data);

    ~MainMenuState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};