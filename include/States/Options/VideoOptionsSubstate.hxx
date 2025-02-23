#pragma once

#include "States/Substate.hxx"
#include "GUI/GUI.hxx"

class VideoOptionsSubstate : public Substate
{
  private:
    sf::RectangleShape background;

    std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons;

    void initGUI();

  public:
    VideoOptionsSubstate(EngineData &data);

    ~VideoOptionsSubstate();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};
