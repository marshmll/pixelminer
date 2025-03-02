#pragma once

#include "States/Substate.hxx"
#include "GUI/GUI.hxx"

class VideoOptionsSubstate : public Substate
{
  private:
    Logger logger;

    sf::RectangleShape background;

    sf::RectangleShape buttonContainer;
    std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::SliderInput>> sliderInputs;

    std::string currentResolution;
    int currentResolutionIndex;

    unsigned int minFramerate;
    unsigned int maxFramerate;

    std::vector<std::string> resolutions;

    void initVariables();

    void initGUI();

    void initButtons();

    void applyVideoSettings();

    const bool saveGraphicsSettings();

  public:
    VideoOptionsSubstate(EngineData &data);

    ~VideoOptionsSubstate();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};
