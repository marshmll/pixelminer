#include "States/Multiplayer/AddServerState.hxx"
#include "stdafx.hxx"

void AddServerState::initGUI()
{
    background.setSize(sf::Vector2f(data.vm->size));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    background.setTexture(&data.activeResourcePack->getTexture("Background"));

    nameInput = std::make_unique<gui::Input>(
        sf::Vector2f(background.getGeometricCenter().x - gui::percent(data.vm->size.x, 40.f) / 2.f,
                     gui::percent(data.vm->size.y, 25.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 8.f)), sf::Color(0, 0, 0, 180),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), gui::percent(data.vm->size.y, 3.f),
        2.f, sf::Color(200, 200, 200, 255), _("Server Name (optional)"), true);

    addressInput = std::make_unique<gui::Input>(
        sf::Vector2f(background.getGeometricCenter().x - gui::percent(data.vm->size.x, 40.f) / 2.f,
                     gui::percent(data.vm->size.y, 40.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 8.f)), sf::Color(0, 0, 0, 180),
        data.activeResourcePack->getFont("Regular"), gui::charSize(*data.vm, 95), gui::percent(data.vm->size.y, 3.f),
        2.f, sf::Color(200, 200, 200, 255), _("Server Address"));

    buttons["Add"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(background.getGeometricCenter().x - gui::percent(data.vm->size.x, 40.f) / 2.f,
                     gui::percent(data.vm->size.y, 60.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), _("Add"), data.activeResourcePack->getFont("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Cancel"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(background.getGeometricCenter().x - gui::percent(data.vm->size.x, 40.f) / 2.f,
                     gui::percent(data.vm->size.y, 68.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 40.f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), _("Cancel"), data.activeResourcePack->getFont("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Add"]->setState(gui::ButtonState::Disabled);
}

const bool AddServerState::addServer()
{
    JArray array;

    if (std::filesystem::exists(SETTINGS_FOLDER + "servers.json"))
    {
        std::ifstream curr_servers_file(SETTINGS_FOLDER + "servers.json");

        if (!curr_servers_file.is_open())
            return false;

        std::stringstream ss;
        ss << curr_servers_file.rdbuf();
        curr_servers_file.close();

        try
        {
            array = JSON::parse(ss.str()).getAs<JArray>();
        }
        catch (std::runtime_error &)
        {
            return false;
        }
    }

    array.push_back(JObject({{"name", nameInput->getValue().empty() ? _("Pixelminer Server") : nameInput->getValue()},
                             {"address", addressInput->getValue()}}));

    std::ofstream new_servers_file(SETTINGS_FOLDER + "servers.json");

    if (!new_servers_file.is_open())
        return false;

    new_servers_file << JSON::stringify(array);

    return new_servers_file.good();
}

AddServerState::AddServerState(EngineData &data) : State(data)
{
    initGUI();
}

AddServerState::~AddServerState() = default;

void AddServerState::update(const float &dt)
{
    updateMousePositions();

    nameInput->update(dt, mousePosView, *data.event);
    addressInput->update(dt, mousePosView, *data.event);

    if (!addressInput->getValue().empty())
        buttons.at("Add")->setState(gui::ButtonState::Idle);
    else
        buttons.at("Add")->setState(gui::ButtonState::Disabled);

    for (auto &[_, button] : buttons)
        button->update(mousePosView);

    if (buttons.at("Cancel")->isPressed())
    {
        killSelf();
    }
    else if (buttons.at("Add")->isPressed())
    {
        if (addServer())
            killSelf();
    }
}

void AddServerState::render(sf::RenderTarget &target)
{
    target.draw(background);
    nameInput->render(target);
    addressInput->render(target);

    for (auto &[_, button] : buttons)
        button->render(target);
}
