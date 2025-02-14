#pragma once

#include "GUI/GUI.hxx"
#include "States/State.hxx"

class AddServerState : public State
{
  private:
    sf::RectangleShape background;
    std::unique_ptr<gui::Input> nameInput;
    std::unique_ptr<gui::Input> addressInput;

    std::unordered_map<std::string, std::unique_ptr<gui::TextButton>> buttons;

    void initGUI();

    const bool addServer();

  public:
    AddServerState(EngineData &data);

    ~AddServerState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};
