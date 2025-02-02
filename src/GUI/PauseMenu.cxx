#include "GUI/PauseMenu.hxx"
#include "stdafx.hxx"

using namespace gui;

PauseMenu::PauseMenu(EngineData &data) : active(false), quit(false), data(data)
{
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setSize((sf::Vector2f(data.vm->size)));
    background.setFillColor(sf::Color(0, 0, 0, 180));

    buttonContainer.setSize(sf::Vector2f(gui::percent(data.vm->size.x, 45.f), gui::percent(data.vm->size.y, 40.f)));
    buttonContainer.setPosition(sf::Vector2f(data.vm->size.x / 2.f - buttonContainer.getSize().x / 2.f,
                                             data.vm->size.y / 2.f - buttonContainer.getSize().y / 2.f));

    float gap = gui::charSize(*data.vm, 110);
    int rows = 5;
    int cols_in = 2;
    int cols_out = 1;
    float btn_width_out = (buttonContainer.getSize().x - (cols_out - 1) * gap) / cols_out;
    float btn_width_in = (buttonContainer.getSize().x - (cols_in - 1) * gap) / cols_in;
    float btn_height = (buttonContainer.getSize().y - (rows - 1) * gap) / rows;

    buttons["BackToGame"] = std::make_unique<gui::TextButton>(
        buttonContainer.getPosition(), sf::Vector2f(btn_width_out, btn_height), sf::Color(200, 200, 200, 200),
        "Back to Game", data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White,
        2.f, sf::Color::Black);

    buttons["Achievements"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x, buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_in, btn_height), sf::Color(200, 200, 200, 200), "Achievements",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Statistics"] =
        std::make_unique<gui::TextButton>(sf::Vector2f(buttonContainer.getPosition().x + btn_width_in + gap,
                                                       buttonContainer.getPosition().y + btn_height + gap),
                                          sf::Vector2f(btn_width_in, btn_height), sf::Color(200, 200, 200, 200),
                                          "Statistics", data.activeResourcePack->fonts.at("Regular"),
                                          gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Options"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x, buttonContainer.getPosition().y + (btn_height + gap) * 3),
        sf::Vector2f(btn_width_in, btn_height), sf::Color(200, 200, 200, 200), "Options",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["OpenToLAN"] =
        std::make_unique<gui::TextButton>(sf::Vector2f(buttonContainer.getPosition().x + btn_width_in + gap,
                                                       buttonContainer.getPosition().y + (btn_height + gap) * 3),
                                          sf::Vector2f(btn_width_in, btn_height), sf::Color(200, 200, 200, 200),
                                          "Open to LAN", data.activeResourcePack->fonts.at("Regular"),
                                          gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["SaveAndQuit"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x, buttonContainer.getPosition().y + (btn_height + gap) * 4),
        sf::Vector2f(btn_width_out, btn_height), sf::Color(200, 200, 200, 200), "Save and Quit to Main Menu",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::update(const float &dt, const sf::Vector2f &mouse_pos)
{
    if (!active)
        return;

    for (auto &[key, button] : buttons)
        button->update(mouse_pos);

    if (buttons.at("BackToGame")->isPressed())
    {
        setActive(false);
    }
    else if (buttons.at("SaveAndQuit")->isPressed())
    {
        setQuit(true);
    }
}

void PauseMenu::render(sf::RenderTarget &target)
{
    if (!active)
        return;

    target.draw(background);

    for (auto &[key, button] : buttons)
        button->render(target);
}

const bool &PauseMenu::isActive() const
{
    return active;
}

const bool &PauseMenu::getQuit() const
{
    return quit;
}

void PauseMenu::toggleActive()
{
    active = !active;
}

void PauseMenu::setActive(const bool &active)
{
    this->active = active;
}

void PauseMenu::setQuit(const bool &quit)
{
    this->quit = quit;
}
