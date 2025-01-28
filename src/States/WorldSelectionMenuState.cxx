#include "States/WorldSelectionMenuState.hxx"
#include "stdafx.hxx"

void WorldSelectionMenuState::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->getTexture("Background"));
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
    int cols_top = 2;
    int cols_bot = 4;
    float btn_width_top = (buttonContainer.getSize().x - (cols_top - 1) * gap) / cols_top;
    float btn_width_bottom = (buttonContainer.getSize().x - (cols_bot - 1) * gap) / cols_bot;
    float btn_height = (buttonContainer.getSize().y - (rows - 1) * gap) / rows;

    buttons["PlaySelectedWorld"] = std::make_unique<gui::TextButton>(
        buttonContainer.getPosition(), sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200),
        "Play Selected World", data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95),
        sf::Color::White, 2.f, sf::Color::Black);

    buttons["CreateNewWorld"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + btn_width_top + gap, buttonContainer.getPosition().y),
        sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200), "Create New World",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Edit"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x, buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), "Edit",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Delete"] =
        std::make_unique<gui::TextButton>(sf::Vector2f(buttonContainer.getPosition().x + btn_width_bottom + gap,
                                                       buttonContainer.getPosition().y + btn_height + gap),
                                          sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200),
                                          "Delete", data.activeResourcePack->fonts.at("Regular"),
                                          gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["ReCreate"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 2) + (gap * 2),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), "Re-Create",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Cancel"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 3) + (gap * 3),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), "Cancel",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons.at("PlaySelectedWorld")->setState(gui::ButtonState::Disabled);
    buttons.at("Edit")->setState(gui::ButtonState::Disabled);
    buttons.at("Delete")->setState(gui::ButtonState::Disabled);
    buttons.at("ReCreate")->setState(gui::ButtonState::Disabled);
}

void WorldSelectionMenuState::initWorldSelectors()
{
    worldSelectorsList = std::make_unique<gui::ScrollList>(
        *data.vm, sf::Vector2f(gui::percent(data.vm->size.x, 60.f), gui::percent(data.vm->size.y, 61.f)),
        sf::Vector2f(data.vm->size.x / 2.f - gui::percent(data.vm->size.x, 60.f) / 2.f,
                     header.getPosition().y + header.getSize().y),
        gui::percent(data.vm->size.x, .5f), sf::Color(150, 150, 150, 200));

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
            worldSelectors.emplace_back(data, metadata, worldSelectorsList->getPosition().y);
        else
            worldSelectors.emplace_back(data, metadata,
                                        worldSelectors.back().getPosition().y + gui::percent(data.vm->size.y, 2.f));
    }
}

WorldSelectionMenuState::WorldSelectionMenuState(EngineData &data) : State(data)
{
    initGUI();
    initWorldSelectors();
}

WorldSelectionMenuState::~WorldSelectionMenuState()
{
}

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
    for (auto &[_, button] : buttons)
        button->update(mousePosView);

    if (buttons.at("Cancel")->isPressed())
        killState();

    worldSelectorsList->update(dt, mousePosView);

    updateMousePositions(worldSelectorsList->getView());
    for (auto &selector : worldSelectors)
        selector.update(dt, mousePosView);
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
        selector.render(target);

    target.setView(target.getDefaultView());

    worldSelectorsList->render(target);
}