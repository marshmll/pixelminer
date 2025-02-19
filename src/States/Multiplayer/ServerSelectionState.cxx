#include "States/Multiplayer/ServerSelectionState.hxx"
#include "stdafx.hxx"

void ServerSelectionState::initGUI()
{
    // Initialize background
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->textures.at("Background"));

    // Initialize header and footer
    header.setSize(sf::Vector2f(data.vm->size.x, gui::percent(data.vm->size.y, 15.f)));
    header.setPosition(sf::Vector2f(0.f, 0.f));
    header.setFillColor(sf::Color(0, 0, 0, 80));

    footer.setSize(sf::Vector2f(data.vm->size.x, gui::percent(data.vm->size.y, 24.f)));
    footer.setPosition(sf::Vector2f(0.f, data.vm->size.y - footer.getSize().y));
    footer.setFillColor(sf::Color(0, 0, 0, 80));

    // Initialize button container
    buttonContainer.setSize(
        sf::Vector2f(gui::percent(footer.getSize().x, 90.f), gui::percent(footer.getSize().y, 60.f)));
    buttonContainer.setPosition(
        sf::Vector2f(footer.getPosition().x + footer.getSize().x / 2.f - buttonContainer.getSize().x / 2.f,
                     footer.getPosition().y + footer.getSize().y / 2.f - buttonContainer.getSize().y / 2.f));

    // Initialize buttons
    initializeButtons();
}

void ServerSelectionState::initializeButtons()
{
    float gap = gui::charSize(*data.vm, 110);
    int rows = 2;
    int cols_top = 3;
    int cols_bot = 4;
    float btn_width_top = (buttonContainer.getSize().x - (cols_top - 1) * gap) / cols_top;
    float btn_width_bottom = (buttonContainer.getSize().x - (cols_bot - 1) * gap) / cols_bot;
    float btn_height = (buttonContainer.getSize().y - (rows - 1) * gap) / rows;

    auto createButton = [&](const std::string &name, const sf::Vector2f &position, const sf::Vector2f &size,
                            const std::string &label) {
        buttons[name] = std::make_unique<gui::TextButton>(
            position, size, sf::Color(200, 200, 200, 200), label, data.activeResourcePack->getFont("Regular"),
            gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);
    };

    createButton("JoinServer", buttonContainer.getPosition(), sf::Vector2f(btn_width_top, btn_height),
                 _("Join Server"));
    createButton("DirectConnect",
                 sf::Vector2f(buttonContainer.getPosition().x + btn_width_top + gap, buttonContainer.getPosition().y),
                 sf::Vector2f(btn_width_top, btn_height), _("Direct Connect"));
    createButton("AddServer",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width_top * 2) + (gap * 2),
                              buttonContainer.getPosition().y),
                 sf::Vector2f(btn_width_top, btn_height), _("Add Server"));
    createButton("Edit",
                 sf::Vector2f(buttonContainer.getPosition().x, buttonContainer.getPosition().y + btn_height + gap),
                 sf::Vector2f(btn_width_bottom, btn_height), _("Edit"));
    createButton("Delete",
                 sf::Vector2f(buttonContainer.getPosition().x + btn_width_bottom + gap,
                              buttonContainer.getPosition().y + btn_height + gap),
                 sf::Vector2f(btn_width_bottom, btn_height), _("Delete"));
    createButton("Refresh",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 2) + (gap * 2),
                              buttonContainer.getPosition().y + btn_height + gap),
                 sf::Vector2f(btn_width_bottom, btn_height), _("Refresh"));
    createButton("Cancel",
                 sf::Vector2f(buttonContainer.getPosition().x + (btn_width_bottom * 3) + (gap * 3),
                              buttonContainer.getPosition().y + btn_height + gap),
                 sf::Vector2f(btn_width_bottom, btn_height), _("Cancel"));

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

    loadServersFromFile();
}

void ServerSelectionState::loadServersFromFile()
{
    std::ifstream servers_file(SETTINGS_FOLDER + "servers.json");
    if (!servers_file.is_open())
        return;

    std::stringstream ss;
    ss << servers_file.rdbuf();
    servers_file.close();

    try
    {
        JArray servers = JSON::parse(ss.str()).getAs<JArray>();
        for (auto &val : servers)
        {
            JObject obj = val.getAs<JObject>();
            ServerMetadata metadata = createServerMetadata(obj);
            addServerSelector(metadata);
        }
    }
    catch (std::runtime_error &e)
    {
        logger.logError(_("Error parsing \"servers.json\": ") + e.what(), false);
        replaceSelf(
            std::make_unique<MessageState>(data, _("Error parsing \"servers.json\" file"),
                                           _("The file may be corrupted or ill-formed. Fix or delete the file:") +
                                               "\n" + (SETTINGS_FOLDER + "servers.json")));
    }
}

ServerMetadata ServerSelectionState::createServerMetadata(JObject &obj)
{
    ServerMetadata metadata;
    metadata.serverName = obj.at("name").getAs<std::string>();
    metadata.serverAddress = obj.at("address").getAs<std::string>();
    metadata.serverDescription = _("Attempting to reach server...");
    metadata.gameVersion = _("Unknown");
    metadata.status = _("Pending");
    metadata.connections = 0;
    metadata.maxConnections = 0;

    return std::move(metadata);
}

void ServerSelectionState::addServerSelector(const ServerMetadata &metadata)
{
    if (serverSelectors.empty())
    {
        serverSelectors.push_back(std::make_unique<ServerSelector>(
            mutex, data, metadata, std::floor(serverSelectorsList->getPosition().y + 1.f), socket, socketSelector,
            data.activeResourcePack->getTexture("Conn")));
    }
    else
    {
        serverSelectors.push_back(std::make_unique<ServerSelector>(
            mutex, data, metadata,
            std::floor(serverSelectors.back()->getPosition().y + serverSelectors.back()->getSize().y +
                       gui::percent(data.vm->size.y, 2.f)),
            socket, socketSelector, data.activeResourcePack->getTexture("Conn")));
    }

    if (!serverSelectors.empty())
    {
        serverSelectorsList->setMaxScrollDelta(
            serverSelectors.back()->getPosition().y + serverSelectors.back()->getSize().y, 10.f);
    }
}

void ServerSelectionState::fetchServerInfo()
{
    threadRunning = true;
    for (auto &selector : serverSelectors)
    {
        selector->metadata.serverDescription = _("Attempting to reach server...");
        selector->metadata.status = _("Pending");
        updateSelectorDescription(selector);
    }

    for (int i = 0; i < serverSelectors.size(); i++)
    {
        if (abortThread)
        {
            threadRunning = false;
            return;
        }
        serverSelectors[i]->fetchData();
    }
    threadRunning = false;
    ready = true;
}

void ServerSelectionState::updateSelectorDescription(const std::shared_ptr<ServerSelector> &selector)
{
    std::string str = selector->metadata.serverDescription + "\n" + _("Status: ") + selector->metadata.status + "\n" +
                      selector->metadata.serverAddress;
    selector->description->setString(sf::String::fromUtf8(str.begin(), str.end()));
    selector->description->setFillColor(sf::Color::Yellow);
}

ServerSelectionState::ServerSelectionState(EngineData &data)
    : State(data), logger("ServerSelectionState"), ready(false), threadRunning(false), abortThread(false)
{
    initGUI();
    initSocket();
    initServerSelectors();
    std::thread(&ServerSelectionState::fetchServerInfo, this).detach();
}

ServerSelectionState::~ServerSelectionState()
{
    while (threadRunning)
        ;
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
    serverSelectorsList->update(dt, mousePosView, *data.event);
    if (serverSelectorsList->isScrollLocked())
        return;

    updateButtons();
    handleButtonActions();
    updateServerSelectors(dt);
}

void ServerSelectionState::updateButtons()
{
    if (!ready)
        buttons.at("Refresh")->setState(gui::ButtonState::Disabled);
    else
        buttons.at("Refresh")->setState(gui::ButtonState::Idle);

    for (auto &[_, button] : buttons)
    {
        button->update(mousePosView);
        if (button->isPressed() && data.activeResourcePack->getSound("Click").getStatus() != sf::Sound::Status::Playing)
            data.activeResourcePack->getSound("Click").play();
    }
}

void ServerSelectionState::handleButtonActions()
{
    if (buttons.at("Cancel")->isPressed())
    {
        abortThread = true;
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
        joinSelectedServer();
    }
    else if (buttons.at("Refresh")->isPressed() && ready)
    {
        ready = false;
        std::thread(&ServerSelectionState::fetchServerInfo, this).detach();
    }
}

void ServerSelectionState::joinSelectedServer()
{
    std::string address = selectedServer.value()->metadata.serverAddress;
    size_t colon_pos = address.find(':');
    const sf::IpAddress ip = sf::IpAddress::resolve(address.substr(0, colon_pos)).value();
    const unsigned short port = std::stoi(address.substr(colon_pos + 1));
    replaceSelf(std::make_shared<ClientGameState>(data, ip, port));
}

void ServerSelectionState::updateServerSelectors(const float &dt)
{
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

    for (auto &[_, button] : buttons)
        button->render(target);

    target.setView(serverSelectorsList->getView());
    for (auto &selector : serverSelectors)
        selector->render(target);
    target.setView(target.getDefaultView());

    serverSelectorsList->render(target);
}