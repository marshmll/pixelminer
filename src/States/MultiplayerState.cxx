#include "States/MultiplayerState.hxx"
#include "stdafx.hxx"

void MultiplayerState::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.textures->at("Stone"));
    background.setFillColor(sf::Color(255, 255, 255, 100));

    header.setSize(sf::Vector2f(data.vm->size.x, gui::percent(data.vm->size.y, 15.f)));
    header.setPosition(sf::Vector2f(0.f, 0.f));
    header.setFillColor(sf::Color(0, 0, 0, 80));

    footer.setSize(sf::Vector2f(data.vm->size.x, gui::percent(data.vm->size.y, 24.f)));
    footer.setPosition(sf::Vector2f(0.f, data.vm->size.y - footer.getSize().y));
    footer.setFillColor(sf::Color(0, 0, 0, 80));

    buttonContainer.setSize(
        sf::Vector2f(gui::percent(footer.getSize().x, 90.f), gui::percent(footer.getSize().y, 60.f)));
    buttonContainer.setPosition(
        sf::Vector2f(footer.getPosition().x + footer.getSize().x / 2.f - buttonContainer.getSize().x / 2.f,
                     footer.getPosition().y + footer.getSize().y / 2.f - buttonContainer.getSize().y / 2.f));

    float gap = gui::charSize(*data.vm, 110);
    int rows = 2;
    int cols_top = 3;
    int cols_bot = 4;
    float btn_width_top = (buttonContainer.getSize().x - (cols_top - 1) * gap) / cols_top;
    float btn_width_bottom = (buttonContainer.getSize().x - (cols_bot - 1) * gap) / cols_bot;
    float btn_height = (buttonContainer.getSize().y - (rows - 1) * gap) / rows;

    buttons["JoinServer"] = std::make_unique<gui::TextButton>(
        buttonContainer.getPosition(), sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200),
        "Join Server", data.fonts->at("MinecraftRegular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["DirectConnect"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + btn_width_top + gap, buttonContainer.getPosition().y),
        sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200), "Direct Connect",
        data.fonts->at("MinecraftRegular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["AddServer"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_top * 2) + (gap * 2),
                     buttonContainer.getPosition().y),
        sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200), "Add Server",
        data.fonts->at("MinecraftRegular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Edit"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x, buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), "Edit",
        data.fonts->at("MinecraftRegular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Delete"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + btn_width_bottom + gap,
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), "Delete",
        data.fonts->at("MinecraftRegular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Refresh"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 2) + (gap * 2),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), "Refresh",
        data.fonts->at("MinecraftRegular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Cancel"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 3) + (gap * 3),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), "Cancel",
        data.fonts->at("MinecraftRegular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons.at("JoinServer")->setState(gui::ButtonState::Disabled);
    buttons.at("Edit")->setState(gui::ButtonState::Disabled);
    buttons.at("Delete")->setState(gui::ButtonState::Disabled);
}

MultiplayerState::MultiplayerState(StateData &data) : State(data)
{
    initGUI();
}

MultiplayerState::~MultiplayerState()
{
}

void MultiplayerState::update(const float &dt)
{
    updateMousePositions();
    updateGUI(dt);
}

void MultiplayerState::render(sf::RenderTarget &target)
{
    renderGUI(target);
}

void MultiplayerState::updateGUI(const float &dt)
{
    for (auto &[_, button] : buttons)
        button->update(mousePosView);

    if (buttons.at("Cancel")->isPressed())
        killState();
}

void MultiplayerState::renderGUI(sf::RenderTarget &target)
{
    target.draw(background);
    target.draw(header);
    target.draw(footer);
    // target.draw(buttonContainer);

    for (auto &[_, button] : buttons)
        button->render(target);
}