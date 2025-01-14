#include "States/DirectConnectState.hxx"
#include "stdafx.hxx"

void DirectConnectState::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.textures->at("Stone"));
    background.setFillColor(sf::Color(255, 255, 255, 100));

    addrInput = std::make_unique<gui::Input>(
        sf::Vector2f(background.getGeometricCenter().x - gui::percent(data.vm->size.x, 40.f),
                     background.getGeometricCenter().y - gui::percent(data.vm->size.y, 8.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 8.f)), sf::Color(0, 0, 0, 180),
        data.fonts->at("MinecraftRegular"), gui::charSize(*data.vm, 95), gui::percent(data.vm->size.y, 3.f), 2.f,
        sf::Color(200, 200, 200, 255));
}

DirectConnectState::DirectConnectState(StateData &data) : State(data)
{
    initGUI();
}

DirectConnectState::~DirectConnectState()
{
}

void DirectConnectState::update(const float &dt)
{
    updateMousePositions();
    addrInput->update(dt, mousePosView, *data.event);
}

void DirectConnectState::render(sf::RenderTarget &target)
{
    target.draw(background);
    addrInput->render(target);
}
