#pragma once

#include "States/MainMenuState.hxx"

class Engine
{
  private:
    /* ATTRIBUTES */

    sf::VideoMode vm;
    sf::RenderWindow window;
    std::optional<sf::Event> event;

    sf::Clock dtClock;
    float dt; // Delta time
    unsigned int gridSize;
    float scale;

    StateData stateData;
    std::stack<std::unique_ptr<State>> states;

    std::map<std::string, sf::Font> fonts;

    /* PRIVATE METHODS */

    void initWindow();

    void initVariables();

    void initFonts();

    void initStateData();

    void initMainMenuState();

    void pollWindowEvents();

    void updateDeltaTime();

  public:
    Engine();
    virtual ~Engine();

    /* PUBLIC METHODS */

    void run();

    void update();

    void render();
};