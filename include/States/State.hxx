#pragma once

class State;

#include "Tiles/TileData.hxx"

typedef struct
{
    unsigned int gridSize;
    float scale;
    std::stack<std::unique_ptr<State>> *states;
    std::map<std::string, sf::Font> *fonts;
    std::map<std::string, sf::Texture> *textures;
    std::map<std::uint32_t, TileData> *tileData;
    sf::RenderWindow *window;
    sf::VideoMode *vm;
} StateData;

class State
{
  private:
  protected:
    StateData &data;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;
    sf::Vector2i mousePosGrid;

    bool dead;

  public:
    State(StateData &data);

    virtual ~State();

    virtual void update(const float &dt) = 0;

    virtual void render(sf::RenderTarget &target) = 0;

    // Kills the current state.
    void killState();

    const bool &isDead() const;

    void updateMousePositions(std::optional<sf::View> view = {});
};