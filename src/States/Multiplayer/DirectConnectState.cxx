#include "States/Multiplayer/DirectConnectState.hxx"
#include "stdafx.hxx"

void DirectConnectState::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->textures.at("Background"));

    addrInput = std::make_unique<gui::Input>(
        sf::Vector2f(background.getGeometricCenter().x - gui::percent(data.vm->size.x, 40.f) / 2.f,
                     gui::percent(data.vm->size.y, 35.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 8.f)), sf::Color(0, 0, 0, 180),
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), gui::percent(data.vm->size.y, 3.f),
        2.f, sf::Color(200, 200, 200, 255), "Server Address");

    buttons["JoinServer"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(background.getGeometricCenter().x - gui::percent(data.vm->size.x, 40.f) / 2.f,
                     gui::percent(data.vm->size.y, 50.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), "Join Server", data.activeResourcePack->fonts.at("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Cancel"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(background.getGeometricCenter().x - gui::percent(data.vm->size.x, 40.f) / 2.f,
                     gui::percent(data.vm->size.y, 58.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), "Cancel", data.activeResourcePack->fonts.at("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons.at("JoinServer")->setState(gui::ButtonState::Disabled);
}

DirectConnectState::DirectConnectState(EngineData &data) : State(data)
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

    for (auto &[key, button] : buttons)
        button->update(mousePosView);

    if (buttons.at("Cancel")->isPressed())
        killSelf();
}

void DirectConnectState::render(sf::RenderTarget &target)
{
    target.draw(background);
    addrInput->render(target);

    for (auto &[key, button] : buttons)
        button->render(target);
}
