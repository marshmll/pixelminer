#include "States/Multiplayer/MultiplayerState.hxx"
#include "stdafx.hxx"

void MultiplayerState::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->textures.at("Background"));
    
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
        "Join Server", data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["DirectConnect"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + btn_width_top + gap, buttonContainer.getPosition().y),
        sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200), "Direct Connect",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["AddServer"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_top * 2) + (gap * 2),
                     buttonContainer.getPosition().y),
        sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200), "Add Server",
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

    buttons["Refresh"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 2) + (gap * 2),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), "Refresh",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Cancel"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 3) + (gap * 3),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), "Cancel",
        data.activeResourcePack->fonts.at("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons.at("JoinServer")->setState(gui::ButtonState::Disabled);
    buttons.at("Edit")->setState(gui::ButtonState::Disabled);
    buttons.at("Delete")->setState(gui::ButtonState::Disabled);
}

void MultiplayerState::initServerSelectors()
{
    serverSelectorsList = std::make_unique<gui::ScrollableContainer>(
        *data.vm, sf::Vector2f(gui::percent(data.vm->size.x, 60.f), gui::percent(data.vm->size.y, 61.f)),
        sf::Vector2f(data.vm->size.x / 2.f - gui::percent(data.vm->size.x, 60.f) / 2.f,
                     header.getPosition().y + header.getSize().y),
        0.f, gui::percent(data.vm->size.x, .5f), sf::Color(150, 150, 150, 200));

    std::ifstream servers_file(SETTINGS_FOLDER + "servers.json");

    if (!servers_file.is_open())
        return;

    std::stringstream ss;
    ss << servers_file.rdbuf();
    servers_file.close();

    JArray servers;

    try
    {
        servers = JSON::parse(ss.str()).getAs<JArray>();
    }
    catch (std::runtime_error &)
    {
        std::cerr << "error" << "\n";
        return;
    }

    int i = 0;
    for (auto &val : servers)
    {
        JObject obj = val.getAs<JObject>();

        ServerMetadata metadata;

        metadata.serverName = obj.at("name").getAs<std::string>();
        metadata.serverAddress = obj.at("address").getAs<std::string>();
        metadata.serverDescription = "A PixelMiner Server.";
        metadata.gameVersion = GAME_VERSION;
        metadata.status = "Online";

        if (serverSelectors.empty())
            serverSelectors.push_back(
                std::make_unique<ServerSelector>(data, metadata, std::floor(serverSelectorsList->getPosition().y)));
        else
            serverSelectors.push_back(std::make_unique<ServerSelector>(
                data, metadata,
                std::floor(serverSelectors.back()->getPosition().y + serverSelectors.back()->getSize().y +
                           gui::percent(data.vm->size.y, 2.f))));
    }

    if (!serverSelectors.empty())
    {
        serverSelectorsList->setMaxScrollDelta(
            serverSelectors.back()->getPosition().y + serverSelectors.back()->getSize().y, 10.f);
    }
}

MultiplayerState::MultiplayerState(EngineData &data) : State(data)
{
    initGUI();
    initServerSelectors();
}

MultiplayerState::~MultiplayerState() = default;

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
    serverSelectorsList->update(dt, mousePosView, *data.event, data.mouseData);

    if (serverSelectorsList->isScrollLocked())
        return;

    for (auto &[_, button] : buttons)
        button->update(mousePosView);

    if (buttons.at("Cancel")->isPressed())
        killSelf();
    else if (buttons.at("AddServer")->isPressed())
        data.states->push(std::make_shared<AddServerState>(data));
    else if (buttons.at("DirectConnect")->isPressed())
        data.states->push(std::make_shared<DirectConnectState>(data));

    updateMousePositions(serverSelectorsList->getView());
    if (mouseButtonPressedWithin(1200.f * dt, sf::Mouse::Button::Left) && !serverSelectorsList->isScrollLocked())
    {
        for (auto &selector : serverSelectors)
        {
            selector->update(dt, mousePosView);
            if (selectedServer.has_value())
            {
                if (selector->isSelected() && selectedServer.value() != selector)
                {
                    selectedServer.value()->setSelected(false);
                    selectedServer = selector;
                }
            }
            else if (selector->isSelected())
            {
                selectedServer = selector;

                // Activate buttons
                for (auto &[_, button] : buttons)
                    button->setState(gui::ButtonState::Idle);
            }
        }
    }
    updateMousePositions();
}

void MultiplayerState::renderGUI(sf::RenderTarget &target)
{
    target.draw(background);
    target.draw(header);
    target.draw(footer);
    // target.draw(buttonContainer);

    for (auto &[_, button] : buttons)
        button->render(target);

    target.setView(serverSelectorsList->getView());

    for (auto &selector : serverSelectors)
        selector->render(target);

    target.setView(target.getDefaultView());

    serverSelectorsList->render(target);
}