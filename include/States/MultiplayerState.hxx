#pragma once

#include "GUI/GUI.hxx"
#include "States/DirectConnectState.hxx"
#include "States/State.hxx"

class MultiplayerState : public State
{
  private:
    sf::RectangleShape background;
    sf::RectangleShape header;
    sf::RectangleShape footer;

    sf::RectangleShape buttonContainer;

    std::map<std::string, std::unique_ptr<gui::TextButton>> buttons;

    void initGUI();

  public:
    MultiplayerState(EngineData &data);

    ~MultiplayerState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);

    void updateGUI(const float &dt);

    void renderGUI(sf::RenderTarget &target);
};