#pragma once

#include "Engine/ResourcePack.hxx"
#include "Entities/Functionalities//AttributeFunctionality.hxx"
#include "GUI/GUI.hxx"

class Hotbar
{
  private:
    AttributeFunctionality &playerAttributes;
    std::shared_ptr<ResourcePack> &resorcePack;
    std::map<std::string, std::unique_ptr<sf::Sprite>> sprites;

    uint8_t maxCols;

    uint8_t healthBarRows;
    uint8_t hungerBarRows;

    unsigned int scale;

    void initSprites();

  public:
    Hotbar(AttributeFunctionality &player_attributes, std::shared_ptr<ResourcePack> &resorce_pack,
           const unsigned int &scale);
    ~Hotbar();

    void update(const float &dt);

    void render(sf::RenderTarget &target);

    void renderHealthBar(sf::RenderTarget &target);
};
