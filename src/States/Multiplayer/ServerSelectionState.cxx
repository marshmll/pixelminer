#include "States/Multiplayer/ServerSelectionState.hxx"
#include "stdafx.hxx"

void ServerSelectionState::initGUI()
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
        _("Join Server"), data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White,
        2.f, sf::Color::Black);

    buttons["DirectConnect"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + btn_width_top + gap, buttonContainer.getPosition().y),
        sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200), _("Direct Connect"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["AddServer"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_top * 2) + (gap * 2),
                     buttonContainer.getPosition().y),
        sf::Vector2f(btn_width_top, btn_height), sf::Color(200, 200, 200, 200), _("Add Server"),
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

    buttons["Refresh"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 2) + (gap * 2),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), _("Refresh"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons["Cancel"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 3) + (gap * 3),
                     buttonContainer.getPosition().y + btn_height + gap),
        sf::Vector2f(btn_width_bottom, btn_height), sf::Color(200, 200, 200, 200), _("Cancel"),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), sf::Color::White, 2.f,
        sf::Color::Black);

    buttons.at("JoinServer")->setState(gui::ButtonState::Disabled);
    buttons.at("Edit")->setState(gui::ButtonState::Disabled);
    buttons.at("Delete")->setState(gui::ButtonState::Disabled);
}

void ServerSelectionState::initSocket()
{
    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
        throw std::runtime_error("Failed to bind to a port.");

    socketSelector.add(socket);
}

void ServerSelectionState::initServerSelectors()
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

        std::string address = obj.at("address").getAs<std::string>();
        size_t colon_pos = address.find(':');
        const sf::IpAddress ipAddress = sf::IpAddress::resolve(address.substr(0, colon_pos)).value();
        const unsigned short portAddress = std::stoi(address.substr(colon_pos + 1));

        sf::Packet packet;
        packet << "INFO";
        if (socket.send(packet, ipAddress, portAddress) != sf::Socket::Status::Done)
        {
            std::cerr << "Error sending packet" << "\n";
        }

        sf::Packet result;
        std::optional<sf::IpAddress> ip;
        unsigned short port;
        std::string status = _("Unreacheable");

        if (socketSelector.wait(sf::seconds(2.f)))
        {
            if (socketSelector.isReady(socket))
            {
                if (socket.receive(result, ip, port) == sf::Socket::Status::Done)
                {
                    std::string header, json;
                    result >> header >> json;
                    if (header == "ACK+INFO")
                    {
                        obj = JSON::parse(json).getAs<JObject>();
                        status = "Online";
                    }
                }
            }
        }

        ServerMetadata metadata;

        metadata.serverName = obj.at("name").getAs<std::string>();
        metadata.serverAddress = obj.at("address").getAs<std::string>();
        metadata.serverDescription =
            status == "Online" ? obj.at("description").getAs<std::string>() : "A Pixelminer Server.";
        metadata.gameVersion = status == "Online" ? obj.at("gameVersion").getAs<std::string>() : "Unknown";
        metadata.status = status;
        metadata.connections =
            status == "Online" ? static_cast<unsigned int>(obj.at("connections").getAs<long long>()) : 0;
        metadata.maxConnections =
            status == "Online" ? static_cast<unsigned int>(obj.at("maxConnections").getAs<long long>()) : 0;

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

ServerSelectionState::ServerSelectionState(EngineData &data) : State(data)
{
    initGUI();
    initSocket();
    initServerSelectors();
}

ServerSelectionState::~ServerSelectionState()
{
    socket.unbind();
}

void ServerSelectionState::update(const float &dt)
{
    updateMousePositions();
    updateGUI(dt);
}

void ServerSelectionState::render(sf::RenderTarget &target)
{
    renderGUI(target);
}

void ServerSelectionState::updateGUI(const float &dt)
{
    serverSelectorsList->update(dt, mousePosView, *data.event, data.mouseData);

    if (serverSelectorsList->isScrollLocked())
        return;

    for (auto &[_, button] : buttons)
    {
        button->update(mousePosView);

        if (button->isPressed() && data.activeResourcePack->getSound("Click").getStatus() != sf::Sound::Status::Playing)
            data.activeResourcePack->getSound("Click").play();
    }

    if (buttons.at("Cancel")->isPressed())
    {
        killSelf();
    }
    else if (buttons.at("AddServer")->isPressed())
    {
        data.states->push(std::make_shared<AddServerState>(data));
    }
    else if (buttons.at("DirectConnect")->isPressed())
    {
        replaceSelf(std::make_shared<DirectConnectState>(data));
    }
    else if (buttons.at("JoinServer")->isPressed() && selectedServer.has_value())
    {
        std::string address = selectedServer.value()->metadata.serverAddress;
        size_t colon_pos = address.find(':');

        const sf::IpAddress ip = sf::IpAddress::resolve(address.substr(0, colon_pos)).value();
        const unsigned short port = std::stoi(address.substr(colon_pos + 1));

        replaceSelf(std::make_shared<ClientGameState>(data, ip, port));
    }

    updateMousePositions(serverSelectorsList->getView());
    if (mouseButtonPressedWithin(200, sf::Mouse::Button::Left) && !serverSelectorsList->isScrollLocked())
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

void ServerSelectionState::renderGUI(sf::RenderTarget &target)
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