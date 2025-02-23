#include "States/Options/OptionsState.hxx"

void OptionsState::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->getTexture("Background"));

    buttonContainer.setSize(sf::Vector2f(gui::percent(data.vm->size.x, 70.f), gui::percent(data.vm->size.y, 70.f)));
    buttonContainer.setPosition(
        sf::Vector2f(data.vm->size.x / 2.f - buttonContainer.getSize().x / 2.f, gui::percent(data.vm->size.y, 10.f)));

    buttons["Done"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(data.vm->size.x / 2.f - gui::percent(data.vm->size.x, 45.f) / 2.f,
                     gui::percent(data.vm->size.y, 85.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 45.f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), _("Done"), data.activeResourcePack->getFont("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);
}

OptionsState::OptionsState(EngineData &data) : State(data)
{
    initGUI();
}

OptionsState::~OptionsState() = default;

void OptionsState::update(const float &dt)
{
    updateMousePositions();

    for (auto &[_, button] : buttons)
    {
        button->update(mousePosView);

        if (button->isPressed() && data.activeResourcePack->getSound("Click").getStatus() != sf::Sound::Status::Playing)
            data.activeResourcePack->getSound("Click").play();
    }

    if (buttons.at("Done")->isPressed())
        killSelf();
}

void OptionsState::render(sf::RenderTarget &target)
{
    target.draw(background);
    target.draw(buttonContainer);

    for (auto &[_, button] : buttons)
        button->render(target);
}
