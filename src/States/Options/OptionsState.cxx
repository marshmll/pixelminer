#include "States/Options/OptionsState.hxx"

void OptionsState::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->getTexture("Background"));

    initButtons();
}

void OptionsState::initButtons()
{
    buttonContainer.setSize(sf::Vector2f(gui::percent(data.vm->size.x, 70.f), gui::percent(data.vm->size.y, 55.f)));
    buttonContainer.setPosition(
        sf::Vector2f(data.vm->size.x / 2.f - buttonContainer.getSize().x / 2.f, gui::percent(data.vm->size.y, 10.f)));

    float gap_x = gui::percent(data.vm->size.x, 2.f);
    float gap_y = gui::percent(data.vm->size.x, 1.2f);

    int rows = 7;
    int cols = 2;

    float btn_width = (buttonContainer.getSize().x - (cols - 1) * gap_x) / cols;
    float btn_height = (buttonContainer.getSize().y - (rows - 1) * gap_y) / rows;

    auto createButton = [&](const std::string &name, const sf::Vector2f &position, const sf::Vector2f &size,
                            const std::string &label) {
        buttons[name] = std::make_unique<gui::TextButton>(
            position, size, sf::Color(200, 200, 200, 200), label, data.activeResourcePack->getFont("Regular"),
            gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);
    };

    createButton("SkinCustomization",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                              buttonContainer.getPosition().y + (btn_height * 2) + (gap_y * 2)),
                 sf::Vector2f(btn_width, btn_height), _("Skin Customization"));

    createButton("MusicAndSounds",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                              buttonContainer.getPosition().y + (btn_height * 2) + (gap_y * 2)),
                 sf::Vector2f(btn_width, btn_height), _("Music and Sounds"));

    createButton("VideoOptions",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                              buttonContainer.getPosition().y + (btn_height * 3) + (gap_y * 3)),
                 sf::Vector2f(btn_width, btn_height), _("Video Options"));

    createButton("Controls",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                              buttonContainer.getPosition().y + (btn_height * 3) + (gap_y * 3)),
                 sf::Vector2f(btn_width, btn_height), _("Controls"));

    createButton("Language",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                              buttonContainer.getPosition().y + (btn_height * 4) + (gap_y * 4)),
                 sf::Vector2f(btn_width, btn_height), _("Language"));

    createButton("ChatOptions",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                              buttonContainer.getPosition().y + (btn_height * 4) + (gap_y * 4)),
                 sf::Vector2f(btn_width, btn_height), _("Chat Options"));

    createButton("ResourcePacks",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                              buttonContainer.getPosition().y + (btn_height * 5) + (gap_y * 5)),
                 sf::Vector2f(btn_width, btn_height), _("Resource Packs"));

    createButton("Accessibility",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                              buttonContainer.getPosition().y + (btn_height * 5) + (gap_y * 5)),
                 sf::Vector2f(btn_width, btn_height), _("Accessibility"));

    createButton("Done",
                 sf::Vector2f(data.vm->size.x / 2.f - gui::percent(data.vm->size.x, 40.f) / 2.f,
                              gui::percent(data.vm->size.y, 85.f)),
                 sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 6.f)), _("Done"));
}

OptionsState::OptionsState(EngineData &data) : State(data)
{
    initGUI();
}

OptionsState::~OptionsState() = default;

void OptionsState::update(const float &dt)
{
    if (substate)
    {
        mouseClock.stop();

        substate.value()->update(dt);

        if (substate.value()->isDead())
            substate.reset();

        else if (substate.value()->askedToRestartAllStates())
            restartStates();

        return;
    }
    else if (!mouseClock.isRunning())
        mouseClock.start();

    updateMousePositions();

    for (auto &[_, button] : buttons)
    {
        button->update(mousePosView);

        if (button->isPressed() && data.activeResourcePack->getSound("Click").getStatus() != sf::Sound::Status::Playing)
            data.activeResourcePack->getSound("Click").play();
    }

    if (mouseButtonPressedWithin(200, sf::Mouse::Button::Left))
    {
        if (buttons.at("Done")->isPressed())
            killSelf();

        else if (buttons.at("VideoOptions")->isPressed())
            createSubstate(std::make_shared<VideoOptionsSubstate>(data));

        else if (buttons.at("Language")->isPressed())
            replaceSelf(std::make_shared<LocalesState>(data));
    }
}

void OptionsState::render(sf::RenderTarget &target)
{
    if (substate)
    {
        substate.value()->render(target);
        return;
    }

    target.draw(background);

    for (auto &[_, button] : buttons)
        button->render(target);
}
