#include "Player/PlayerGUI.hxx"
#include "stdafx.hxx"

void PlayerGUI::initGUI()
{
    hotbar = std::make_unique<Hotbar>(player.getAttributeFunctionality(), resourcePack, vm, scale);
}

PlayerGUI::PlayerGUI(Player &player, std::shared_ptr<ResourcePack> &resource_pack, sf::VideoMode &vm,
                     const unsigned int &scale)
    : player(player), resourcePack(resource_pack), vm(vm), scale(scale)
{
    initGUI();
}

PlayerGUI::~PlayerGUI() = default;

void PlayerGUI::update(const float &dt)
{
    hotbar->update(dt);
}

void PlayerGUI::render(sf::RenderTarget &target)
{
    hotbar->render(target);
}
