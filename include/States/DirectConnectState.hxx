#pragma once

#include "GUI/GUI.hxx"
#include "States/State.hxx"

class DirectConnectState : public State
{
  private:
    sf::RectangleShape background;
    std::unique_ptr<gui::Input> addrInput;
    std::map<std::string, std::unique_ptr<gui::TextButton>> buttons;

    void initGUI();

  public:
    DirectConnectState(EngineData &data);
    ~DirectConnectState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};
