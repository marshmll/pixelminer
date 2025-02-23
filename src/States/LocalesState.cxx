#include "stdafx.hxx"
#include "States/LocalesState.hxx"

void LocalesState::readLocales()
{
    try
    {
        localesObj = JSON::parse(std::filesystem::path(SETTINGS_FOLDER + "locales.json")).getAs<JObject>();
    }
    catch (std::runtime_error &e)
    {
        replaceSelf(std::make_shared<MessageState>(data, "Failed to read locales file", e.what()));
    }
}

void LocalesState::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->getTexture("Background"));

    header.setSize(sf::Vector2f(data.vm->size.x, gui::percent(data.vm->size.y, 15.f)));
    header.setPosition(sf::Vector2f(0.f, 0.f));
    header.setFillColor(sf::Color(0, 0, 0, 80));

    footer.setSize(sf::Vector2f(data.vm->size.x, gui::percent(data.vm->size.y, 24.f)));
    footer.setPosition(sf::Vector2f(0.f, data.vm->size.y - footer.getSize().y));
    footer.setFillColor(sf::Color(0, 0, 0, 80));

    buttonContainer.setSize(sf::Vector2f(gui::percent(footer.getSize().x, 70.f), gui::percent(data.vm->size.y, 6.f)));
    buttonContainer.setPosition(
        sf::Vector2f(footer.getPosition().x + footer.getSize().x / 2.f - buttonContainer.getSize().x / 2.f,
                     footer.getPosition().y + footer.getSize().y / 2.f - buttonContainer.getSize().y / 2.f));

    float gap = gui::charSize(*data.vm, 110);
    int rows = 1;
    int cols = 2;
    float btn_width = (buttonContainer.getSize().x - (cols - 1) * gap) / cols;
    float btn_height = (buttonContainer.getSize().y - (rows - 1) * gap) / rows;

    buttons["Done"] = std::make_unique<gui::TextButton>(
        buttonContainer.getPosition(), sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Done"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Cancel"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + btn_width + gap, buttonContainer.getPosition().y),
        sf::Vector2f(btn_width, btn_height), sf::Color(200, 200, 200, 200), _("Cancel"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons.at("Done")->setState(gui::ButtonState::Disabled);

    scrollContainer = std::make_unique<gui::ScrollableContainer>(
        *data.vm, sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 61.f)),
        sf::Vector2f(data.vm->size.x / 2.f - gui::percent(data.vm->size.x, 40.f) / 2.f,
                     header.getPosition().y + header.getSize().y),
        0.f, gui::percent(data.vm->size.x, 1.2f), sf::Color(200, 200, 200, 255));

    try
    {
        for (auto &val : localesObj.at("available").getAs<JArray>())
        {
            JObject localeObj = val.getAs<JObject>();
            std::string name = localeObj.at("name").getAs<std::string>(),
                        locale = localeObj.at("locale").getAs<std::string>();

            if (options.empty())
            {
                options.emplace_back(sf::String::fromUtf8(name.begin(), name.end()), locale,
                                     data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 90),
                                     scrollContainer->getPosition(),
                                     sf::Vector2f(scrollContainer->getSize().x, gui::percent(data.vm->size.y, 4.f)));
            }
            else
            {
                options.emplace_back(sf::String::fromUtf8(name.begin(), name.end()), locale,
                                     data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 90),
                                     sf::Vector2f(options.back().getPosition().x,
                                                  options.back().getPosition().y + options.back().getSize().y +
                                                      gui::percent(options.back().getSize().y, 20.f)),
                                     sf::Vector2f(scrollContainer->getSize().x, gui::percent(data.vm->size.y, 4.f)));
            }
        }
    }
    catch (std::runtime_error &e)
    {
        replaceSelf(std::make_shared<MessageState>(
            data, _("Invalid locales file"),
            static_cast<std::string>(e.what()) + "\n" +
                _("The file \"locales.json\" in SETTINGS_FOLDER might be corrupted or ill-formed.")));
    }

    scrollContainer->setMaxScrollDelta(options.back().getPosition().y + options.back().getSize().y, 5.f);
}

const bool LocalesState::saveNewLocale()
{
    localesObj.at("current") = selectedOption->getValue();

    std::ofstream locales_file(SETTINGS_FOLDER + "locales.json");

    if (!locales_file.is_open())
    {
        replaceSelf(
            std::make_shared<MessageState>(data, _("Failed to save locale files"),
                                           _("Could not open \"locales.json\" to save the new language preference.")));
        return false;
    }

    locales_file << JSON::stringify(localesObj);

    *data.currentLocale = selectedOption->getValue();

    return true;
}

LocalesState::LocalesState(EngineData &data) : State(data), selectedOption(nullptr)
{
    readLocales();
    initGUI();
}

LocalesState::~LocalesState() = default;

void LocalesState::update(const float &dt)
{
    updateMousePositions();

    scrollContainer->update(dt, mousePosView, data.mouseData);

    if (scrollContainer->isScrollLocked())
        return;

    if (!selectedOption)
        buttons.at("Done")->setState(gui::ButtonState::Disabled);
    else
        buttons.at("Done")->setState(gui::ButtonState::Idle);

    for (auto &[_, button] : buttons)
    {
        button->update(mousePosView);

        if (button->isPressed() && data.activeResourcePack->getSound("Click").getStatus() != sf::Sound::Status::Playing)
            data.activeResourcePack->getSound("Click").play();
    }

    if (buttons.at("Cancel")->isPressed())
    {
        setup_i18n(*data.currentLocale);
        killSelf();
    }
    else if (buttons.at("Done")->isPressed())
    {
        if (saveNewLocale())
        {
            restartStates();
            return;
        }
    }

    updateMousePositions(scrollContainer->getView());
    for (auto &option : options)
    {
        option.update(mousePosView);

        if (option.isSelected())
        {
            if (!selectedOption)
            {
                selectedOption = &option;
                setup_i18n(option.getValue());

                buttons["Done"] = std::make_unique<gui::TextButton>(
                    buttons["Done"]->getPosition(), buttons["Done"]->getSize(), sf::Color(200, 200, 200, 200),
                    _("Done"), data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95),
                    sf::Color::White, 2.f, sf::Color::Black);

                buttons["Cancel"] = std::make_unique<gui::TextButton>(
                    buttons["Cancel"]->getPosition(), buttons["Cancel"]->getSize(), sf::Color(200, 200, 200, 200),
                    _("Cancel"), data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95),
                    sf::Color::White, 2.f, sf::Color::Black);
            }
            else if (selectedOption->getValue() != option.getValue())
            {
                selectedOption->setSelected(false);
                selectedOption = &option;
                setup_i18n(option.getValue());

                buttons["Done"] = std::make_unique<gui::TextButton>(
                    buttons["Done"]->getPosition(), buttons["Done"]->getSize(), sf::Color(200, 200, 200, 200),
                    _("Done"), data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95),
                    sf::Color::White, 2.f, sf::Color::Black);

                buttons["Cancel"] = std::make_unique<gui::TextButton>(
                    buttons["Cancel"]->getPosition(), buttons["Cancel"]->getSize(), sf::Color(200, 200, 200, 200),
                    _("Cancel"), data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95),
                    sf::Color::White, 2.f, sf::Color::Black);
            }
        }
    }
}

void LocalesState::render(sf::RenderTarget &target)
{
    target.draw(background);
    target.draw(header);
    target.draw(footer);

    scrollContainer->render(target);

    for (auto &[_, button] : buttons)
        button->render(target);

    target.setView(scrollContainer->getView());

    for (auto &option : options)
        option.render(target);

    target.setView(target.getDefaultView());
}
