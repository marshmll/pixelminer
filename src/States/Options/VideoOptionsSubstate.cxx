#include "States/Options/VideoOptionsSubstate.hxx"

void VideoOptionsSubstate::initVariables()
{
    currentResolution = std::to_string(data.vm->size.x) + "x" + std::to_string(data.vm->size.y);
    currentResolutionIndex = 0;
    minFramerate = 15;
    maxFramerate = 200;

    int index = 0;
    for (int i = sf::VideoMode::getFullscreenModes().size() - 1; i >= 0; i--)
    {
        std::string res = std::to_string(sf::VideoMode::getFullscreenModes()[i].size.x) + "x" +
                          std::to_string(sf::VideoMode::getFullscreenModes()[i].size.y);

        auto it = std::find(resolutions.begin(), resolutions.end(), res);

        if (it == resolutions.end())
        {
            resolutions.emplace_back(res);

            if (res == currentResolution)
                currentResolutionIndex = index;

            index++;
        }
    }
}

void VideoOptionsSubstate::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->getTexture("Background"));

    initButtons();
}

void VideoOptionsSubstate::initButtons()
{
    buttonContainer.setSize(sf::Vector2f(gui::percent(data.vm->size.x, 70.f), gui::percent(data.vm->size.y, 70.f)));
    buttonContainer.setPosition(sf::Vector2f(data.vm->size.x / 2.f - buttonContainer.getSize().x / 2.f,
                                             data.vm->size.y / 2.f - buttonContainer.getSize().y / 2.f));

    float gap_x = gui::percent(data.vm->size.x, 2.f);
    float gap_y = gui::percent(data.vm->size.x, 1.2f);

    int rows = 9;
    int cols = 2;

    float btn_width = (buttonContainer.getSize().x - (cols - 1) * gap_x) / cols;
    float btn_height = (buttonContainer.getSize().y - (rows - 1) * gap_y) / rows;

    sliderInputs["Resolution"] = std::make_unique<gui::SliderInput>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                     buttonContainer.getPosition().y + (btn_height * 0) + (gap_y * 0)),
        sf::Vector2f(btn_width, btn_height), sf::Color(100, 100, 100, 255), sf::Color::Black, 2.f,
        gui::percent(data.vm->size.x, 1.f), sf::Color(200, 200, 200, 200), sf::Color::Black, 2.f,
        data.activeResourcePack->getFont("Regular"), _("Resolution: "), gui::charSize(*data.vm, 95), resolutions.size(),
        currentResolutionIndex);

    sliderInputs["FramerateLimit"] = std::make_unique<gui::SliderInput>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                     buttonContainer.getPosition().y + (btn_height * 0) + (gap_y * 0)),
        sf::Vector2f(btn_width, btn_height), sf::Color(100, 100, 100, 255), sf::Color::Black, 2.f,
        gui::percent(data.vm->size.x, 1.f), sf::Color(200, 200, 200, 200), sf::Color::Black, 2.f,
        data.activeResourcePack->getFont("Regular"), _("Framerate Limit: "), gui::charSize(*data.vm, 95),
        maxFramerate - minFramerate + 2,
        data.gfx->framerateLimit == 0 ? maxFramerate - minFramerate + 2 - 1 : data.gfx->framerateLimit - minFramerate);

    buttons["Fullscreen"] = std::make_unique<gui::ToggleButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                     buttonContainer.getPosition().y + (btn_height * 1) + (gap_y * 1)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Fullscreen"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White,
        data.gfx->fullscreen, 200, 2.f, sf::Color::Black);

    buttons["VSync"] = std::make_unique<gui::ToggleButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                     buttonContainer.getPosition().y + (btn_height * 1) + (gap_y * 1)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("V-Sync"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, data.gfx->vsync,
        200, 2.f, sf::Color::Black);

    buttons["FontSmoothness"] = std::make_unique<gui::ToggleButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                     buttonContainer.getPosition().y + (btn_height * 2) + (gap_y * 2)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Font Smoothness"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White,
        data.gfx->fontSmoothness, 200, 2.f, sf::Color::Black);

    buttons["TextureSmoothness"] = std::make_unique<gui::ToggleButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                     buttonContainer.getPosition().y + (btn_height * 2) + (gap_y * 2)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Texture Smoothness"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White,
        data.gfx->textureSmoothness, 200, 2.f, sf::Color::Black);

    buttons["ApplyAndClose"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 0) + (gap_x * 0),
                     buttonContainer.getPosition().y + (btn_height * 8) + (gap_y * 8)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Apply and Close"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Back"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width * 1) + (gap_x * 1),
                     buttonContainer.getPosition().y + (btn_height * 8) + (gap_y * 8)),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Back"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    if (sliderInputs.at("FramerateLimit")->getSelectedOption() == sliderInputs.at("FramerateLimit")->getMaxOption() - 1)
    {
        sliderInputs.at("FramerateLimit")->setString(_("Framerate Limit: ") + "∞" + _(" fps"));
    }
    else
    {
        sliderInputs.at("FramerateLimit")
            ->setString(_("Framerate Limit: ") +
                        std::to_string(sliderInputs.at("FramerateLimit")->getSelectedOption() + minFramerate) +
                        _(" fps"));
    }
}

void VideoOptionsSubstate::applyVideoSettings()
{
    unsigned int framerate_limit = 0;

    if (sliderInputs.at("FramerateLimit")->getSelectedOption() < sliderInputs.at("FramerateLimit")->getMaxOption() - 1)
    {
        framerate_limit = sliderInputs.at("FramerateLimit")->getSelectedOption() + minFramerate;
    }

    data.gfx->framerateLimit = framerate_limit;
    data.gfx->fullscreen = dynamic_cast<gui::ToggleButton *>(buttons.at("Fullscreen").get())->isOn();
    data.gfx->vsync = dynamic_cast<gui::ToggleButton *>(buttons.at("VSync").get())->isOn();
    data.gfx->screenWidth = std::stoi(currentResolution.substr(0, currentResolution.find("x")));
    data.gfx->screenHeight = std::stoi(currentResolution.substr(currentResolution.find("x") + 1));
    data.gfx->fontSmoothness = dynamic_cast<gui::ToggleButton *>(buttons.at("FontSmoothness").get())->isOn();
    data.gfx->textureSmoothness = dynamic_cast<gui::ToggleButton *>(buttons.at("TextureSmoothness").get())->isOn();

    *data.vm = sf::VideoMode(sf::Vector2u(data.gfx->screenWidth, data.gfx->screenHeight));

    if (data.gfx->fullscreen)
    {
        data.window->create(*data.vm, "PixelMiner " + static_cast<std::string>(GAME_VERSION), sf::State::Fullscreen);
    }
    else
    {
        data.window->create(*data.vm, "PixelMiner " + static_cast<std::string>(GAME_VERSION),
                            sf::Style::Close | sf::Style::Titlebar);

        data.window->setPosition(
            sf::Vector2i(sf::VideoMode::getDesktopMode().size.x / 2 - data.window->getSize().x / 2,
                         sf::VideoMode::getDesktopMode().size.y / 2 - data.window->getSize().y / 2));
    }

    data.window->setFramerateLimit(data.gfx->framerateLimit);
    data.window->setVerticalSyncEnabled(data.gfx->vsync);
    *data.scale = std::max(1u, static_cast<unsigned int>(std::roundf((data.vm->size.x + data.vm->size.y) / 693.f)));

    sf::Image icon;

    if (!icon.loadFromFile((GLOBAL_FOLDER + "Default/icon_5x.png")))
        logger.logWarning(_("Failed to load window icon. Using OS default icon instead."));

    else
        data.window->setIcon(icon);

    if (!data.gfx->saveToFile(SETTINGS_FOLDER + "graphics.json"))
        logger.logWarning(_("Failed to save new video settings to file: graphics.json"));

    data.activeResourcePack->setFontSmoothness(data.gfx->fontSmoothness);
    data.activeResourcePack->setTextureSmoothness(data.gfx->textureSmoothness);
}

VideoOptionsSubstate::VideoOptionsSubstate(EngineData &data) : logger("VideoOptionsSubstate"), Substate(data)
{
    initVariables();
    initGUI();
}

VideoOptionsSubstate::~VideoOptionsSubstate() = default;

void VideoOptionsSubstate::update(const float &dt)
{
    updateMousePositions();

    for (auto &[_, button] : buttons)
    {
        button->update(mousePosView);

        if (button->isPressed() && data.activeResourcePack->getSound("Click").getStatus() != sf::Sound::Status::Playing)
            data.activeResourcePack->getSound("Click").play();
    }

    if (buttons.at("Back")->isPressed())
        killSelf();
    else if (buttons.at("ApplyAndClose")->isPressed())
    {
        applyVideoSettings();
        restartStates();
    }

    for (auto &[_, input] : sliderInputs)
        input->update(mousePosView);

    sliderInputs.at("Resolution")
        ->setString(_("Resolution: ") + resolutions[sliderInputs.at("Resolution")->getSelectedOption()]);

    currentResolution = resolutions[sliderInputs.at("Resolution")->getSelectedOption()];

    if (sliderInputs.at("FramerateLimit")->getSelectedOption() == sliderInputs.at("FramerateLimit")->getMaxOption() - 1)
    {
        sliderInputs.at("FramerateLimit")->setString(_("Framerate Limit: ") + "∞" + _(" fps"));
    }
    else
    {
        sliderInputs.at("FramerateLimit")
            ->setString(_("Framerate Limit: ") +
                        std::to_string(sliderInputs.at("FramerateLimit")->getSelectedOption() + minFramerate) +
                        _(" fps"));
    }
}

void VideoOptionsSubstate::render(sf::RenderTarget &target)
{
    target.draw(background);

    for (auto &[_, button] : buttons)
        button->render(target);

    for (auto &[_, input] : sliderInputs)
        input->render(target);
}
