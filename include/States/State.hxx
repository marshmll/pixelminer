#pragma once

class State;

#include "Engine/ResourcePack.hxx"

struct EngineData
{
    std::string uuid;
    unsigned int gridSize;
    unsigned int scale;
    std::stack<std::unique_ptr<State>> *states;
    std::map<std::string, ResourcePack> *resourcePacks;
    std::shared_ptr<ResourcePack> activeResourcePack;
    sf::RenderWindow *window;
    sf::VideoMode *vm;
    std::optional<sf::Event> *event;
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

    sf::Clock keyClock;

  public:
    State(EngineData &data);

    virtual ~State();

    virtual void update(const float &dt) = 0;

    virtual void render(sf::RenderTarget &target) = 0;

    // Kills the current state.
    virtual void killState();

    const bool &isDead() const;

    void updateMousePositions(std::optional<sf::View> view = {});

    const bool keyPressedWithin(const std::int32_t &milisseconds, const sf::Keyboard::Key &key);
};