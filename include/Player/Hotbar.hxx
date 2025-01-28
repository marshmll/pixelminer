#pragma once

#include "Engine/ResourcePack.hxx"
#include "Entities/Functionalities//AttributeFunctionality.hxx"
#include "GUI/GUI.hxx"

class Hotbar
{
  private:
    AttributeFunctionality &playerAttributes;
    std::shared_ptr<ResourcePack> &resourcePack;
    sf::VideoMode &vm;

    sf::RectangleShape hotbarContainer;

    uint8_t maxCols;

    uint8_t healthBarRows;
    uint8_t hungerBarRows;

    unsigned int scale;

    std::vector<sf::Sprite> healthBar;
    std::vector<sf::Sprite> hungerBar;

    void initGUI();

  public:
    Hotbar(AttributeFunctionality &player_attributes, std::shared_ptr<ResourcePack> &resorce_pack, sf::VideoMode &vm,
           const unsigned int &scale);

    ~Hotbar();

    void update(const float &dt);

    void render(sf::RenderTarget &target);

    void updateHealthBar();

    void updateHungerBar();

    void renderHealthBar(sf::RenderTarget &target);

    void renderHungerBar(sf::RenderTarget &target);
};
