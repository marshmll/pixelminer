#pragma once

#include "Engine/ResourcePack.hxx"
#include "Entities/Playable/Player.hxx"
#include "Player/Hotbar.hxx"

class PlayerGUI
{
  private:
    Player &player;
    std::unique_ptr<Hotbar> hotbar;
    std::shared_ptr<ResourcePack> &resourcePack;
    sf::VideoMode &vm;
    unsigned int scale;

    void initGUI();

  public:
    PlayerGUI(Player &player, std::shared_ptr<ResourcePack> &resource_pack, sf::VideoMode &vm,
              const unsigned int &scale);

    ~PlayerGUI();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};
