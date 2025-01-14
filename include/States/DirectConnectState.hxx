#pragma once

#include "States/State.hxx"
#include "GUI/GUI.hxx"

class DirectConnectState : public State
{
  private:
    sf::RectangleShape background;
    std::unique_ptr<gui::Input> addrInput;

    void initGUI();

  public:
    DirectConnectState(StateData &data);
    ~DirectConnectState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};
