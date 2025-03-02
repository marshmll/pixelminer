#include "States/Singleplayer/WorldSelectionMenuState.hxx"
#include "stdafx.hxx"

void WorldSelectionMenuState::initGUI()
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

    buttonContainer.setSize(
        sf::Vector2f(gui::percent(footer.getSize().x, 90.f), gui::percent(footer.getSize().y, 60.f)));
    buttonContainer.setPosition(
        sf::Vector2f(footer.getPosition().x + footer.getSize().x / 2.f - buttonContainer.getSize().x / 2.f,
                     footer.getPosition().y + footer.getSize().y / 2.f - buttonContainer.getSize().y / 2.f));

    float gap = gui::charSize(*data.vm, 110);
    int rows = 2;
    int cols_top = 2;
    int cols_bot = 4;
    float btn_width_top = (buttonContainer.getSize().x - (cols_top - 1) * gap) / cols_top;
    float btn_width_bottom = (buttonContainer.getSize().x - (cols_bot - 1) * gap) / cols_bot;
    float btn_height = (buttonContainer.getSize().y - (rows - 1) * gap) / rows;

    buttons["PlaySelectedWorld"] = std::make_unique<gui::TextButton>(
        buttonContainer.getPosition(), sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200),
        _("Play Selected World"), data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95),
        sf::Color::White, 2.f, sf::Color::Black);

    buttons["CreateNewWorld"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + btn_width_top + gap, buttonContainer.getPosition().y),
        sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200), _("Create New World"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Edit"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x, buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), _("Edit"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Delete"] =
        std::make_unique<gui::TextButton>(sf::Vector2f(buttonContainer.getPosition().x + btn_width_bottom + gap,
                                                       buttonContainer.getPosition().y + btn_height + gap),
                                          sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200),
                                          _("Delete"), data.activeResourcePack->getFont("Regular"),
                                          gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["ReCreate"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 2) + (gap * 2),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), _("Re-Create"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Cancel"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 3) + (gap * 3),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), _("Cancel"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons.at("PlaySelectedWorld")->setState(gui::ButtonState::Disabled);
    buttons.at("Edit")->setState(gui::ButtonState::Disabled);
    buttons.at("Delete")->setState(gui::ButtonState::Disabled);
    buttons.at("ReCreate")->setState(gui::ButtonState::Disabled);
}

void WorldSelectionMenuState::initWorldSelectors()
{
    worldSelectorsList = std::make_unique<gui::ScrollableContainer>(
        *data.vm, sf::Vector2f(gui::percent(data.vm->size.x, 60.f), gui::percent(data.vm->size.y, 61.f)),
        sf::Vector2f(data.vm->size.x / 2.f - gui::percent(data.vm->size.x, 60.f) / 2.f,
                     header.getPosition().y + header.getSize().y),
        0.f, gui::percent(data.vm->size.x, .5f), sf::Color(150, 150, 150, 200));

    int i = 0;
    for (auto const &entry : std::filesystem::directory_iterator(MAPS_FOLDER))
    {
        WorldMetadata metadata;
        std::ifstream metadataFile(entry.path() / "metadata.json");

        if (!metadataFile.is_open())
            continue;

        std::stringstream ss;
        ss << metadataFile.rdbuf();

        JObject metadataObj = JSON::parse(ss.str()).getAs<JObject>();

        metadata.worldName = metadataObj.at("name").getAs<std::string>();
        metadata.folderName = entry.path().filename().string();
        metadata.creationDate = metadataObj.at("creationDate").getAs<long long>();
        metadata.lastPlayed = metadataObj.at("lastPlayed").getAs<long long>();
        metadata.gameVersion = metadataObj.at("gameVersion").getAs<std::string>();
        metadata.difficulty = metadataObj.at("difficulty").getAs<std::string>();

        if (worldSelectors.empty())
            worldSelectors.push_back(
                std::make_unique<WorldSelector>(data, metadata, std::floor(worldSelectorsList->getPosition().y + 1.f)));
        else
            worldSelectors.push_back(std::make_unique<WorldSelector>(data, metadata,
                                                                     std::floor(worldSelectors.back()->getPosition().y +
                                                                                worldSelectors.back()->getSize().y +
                                                                                gui::percent(data.vm->size.y, 2.f))));

        metadataFile.close();
    }

    if (!worldSelectors.empty())
    {
        worldSelectorsList->setMaxScrollDelta(
            worldSelectors.back()->getPosition().y + worldSelectors.back()->getSize().y, 10.f);
    }
}

WorldSelectionMenuState::WorldSelectionMenuState(EngineData &data) : State(data)
{
    initGUI();
    initWorldSelectors();
}

WorldSelectionMenuState::~WorldSelectionMenuState() = default;

void WorldSelectionMenuState::update(const float &dt)
{
    updateMousePositions();
    updateGUI(dt);
}

void WorldSelectionMenuState::render(sf::RenderTarget &target)
{
    renderGUI(target);
}

void WorldSelectionMenuState::updateGUI(const float &dt)
{
    worldSelectorsList->update(dt, mousePosView, data.mouseData);

    if (worldSelectorsList->isScrollLocked())
        return;

    for (auto &[_, button] : buttons)
    {
        button->update(mousePosView);

        if (button->isPressed() && data.activeResourcePack->getSound("Click").getStatus() != sf::Sound::Status::Playing)
            data.activeResourcePack->getSound("Click").play();
    }

    if (buttons.at("Cancel")->isPressed())
        killSelf();
    if (buttons.at("PlaySelectedWorld")->isPressed() && selectedWorld.has_value())
    {
        data.activeResourcePack->stopAllMusics();
        replaceSelf(std::make_shared<GameState>(data, selectedWorld.value()->metadata.folderName));
    }

    updateMousePositions(worldSelectorsList->getView());
    if (mouseButtonPressedWithin(200, sf::Mouse::Button::Left) && !worldSelectorsList->isScrollLocked())
    {
        for (auto &selector : worldSelectors)
        {
            selector->update(dt, mousePosView);
            if (selectedWorld.has_value())
            {
                if (selector->isSelected() && selectedWorld.value() != selector)
                {
                    selectedWorld.value()->setSelected(false);
                    selectedWorld = selector;
                }
            }
            else if (selector->isSelected())
            {
                selectedWorld = selector;

                // Activate buttons
                for (auto &[_, button] : buttons)
                    button->setState(gui::ButtonState::Idle);
            }
        }
    }
    updateMousePositions();
}

void WorldSelectionMenuState::renderGUI(sf::RenderTarget &target)
{
    target.draw(background);
    target.draw(header);
    target.draw(footer);

    for (auto &[_, button] : buttons)
        button->render(target);

    target.setView(worldSelectorsList->getView());

    for (auto &selector : worldSelectors)
        selector->render(target);

    target.setView(target.getDefaultView());

    worldSelectorsList->render(target);
}
