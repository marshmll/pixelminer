#pragma once

#include "GUI/GUI.hxx"
#include "States/GameState.hxx"
#include "States/MultiplayerState.hxx"
#include "States/SingleplayerState.hxx"

class MainMenuState : public State
{
  private:
    sf::Texture bgTexture;
    sf::RectangleShape bgShape;
    std::unique_ptr<sf::Text> title;

    std::map<std::string, std::unique_ptr<gui::Button>> buttons;

    void initGUI();

  public:
    MainMenuState(EngineData &data);

    ~MainMenuState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};