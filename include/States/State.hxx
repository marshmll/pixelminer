#pragma once

class State;

#include "Engine/ResourcePack.hxx"

struct EngineData
{
    std::string uuid;
    unsigned int gridSize;
    unsigned int scale;
    std::stack<std::shared_ptr<State>> *states;
    std::unordered_map<std::string, ResourcePack> *resourcePacks;
    std::shared_ptr<ResourcePack> activeResourcePack;
    sf::RenderWindow *window;
    sf::VideoMode *vm;
    std::optional<sf::Event> *event;
    sf::Event::MouseWheelScrolled mouseData; // Only access after checking if event is mouse wheel scroll
};

class State
{
  private:
  protected:
    EngineData &data;

    sf::RenderTexture renderTexture;
    sf::Sprite renderSprite;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;
    sf::Vector2i mousePosGrid;

    bool dead;
    bool replaced;

    sf::Clock keyClock;
    sf::Clock mouseClock;

    std::shared_ptr<State> replacerState;

  public:
    State(EngineData &data);

    virtual ~State();

    virtual void update(const float &dt);

    virtual void render(sf::RenderTarget &target);

    void updateMousePositions(std::optional<sf::View> view = {});

    const bool keyPressedWithin(const std::int32_t &milliseconds, const sf::Keyboard::Key &key);

    const bool mouseButtonPressedWithin(const std::int32_t &milliseconds, const sf::Mouse::Button &button);

    // Kills the current state.
    virtual void killSelf();

    // Kills current state and replace its frame with another State in stack.
    void replaceSelf(std::shared_ptr<State> state);

    std::shared_ptr<State> &getReplacerState();

    const bool &isDead() const;

    const bool &wasReplaced() const;
};