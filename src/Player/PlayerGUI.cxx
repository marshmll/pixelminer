#include "Player/PlayerGUI.hxx"
#include "stdafx.hxx"

void PlayerGUI::initGUI()
{
    hotbar = std::make_unique<Hotbar>(player.getAttributeFunctionality(), resourcePack, scale);
}

PlayerGUI::PlayerGUI(Player &player, std::shared_ptr<ResourcePack> &resource_pack, const unsigned int &scale)
    : player(player), resourcePack(resource_pack), scale(scale)
{
    initGUI();
}

PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::update(const float &dt)
{
}

void PlayerGUI::render(sf::RenderTarget &target)
{
    hotbar->render(target);
}
