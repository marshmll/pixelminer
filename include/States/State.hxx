#pragma once

class State;

#include "Tiles/TileData.hxx"

struct EngineData
{
    unsigned int gridSize;
    float scale;
    std::stack<std::unique_ptr<State>> *states;
    std::map<std::string, sf::Font> *fonts;
    std::map<std::string, sf::Texture> *textures;
    std::map<std::uint32_t, TileData> *tileData;
    std::map<std::string, sf::Shader> *shaders;
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
    void killState();

    const bool &isDead() const;

    void updateMousePositions(std::optional<sf::View> view = {});

    const bool keyPressedWithin(const std::int32_t &milisseconds, const sf::Keyboard::Key &key);
};