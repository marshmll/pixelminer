#include "States/MainMenuState.hxx"
#include "stdafx.hxx"

void MainMenuState::initGUI()
{
    bgShape.setPosition({0.f, 0.f});
    bgShape.setSize(sf::Vector2f(data.vm->size.x, data.vm->size.y));
    bgShape.setTexture(&data.activeResourcePack->textures.at("Background"));

    // TITLE
    title =
        std::make_unique<sf::Text>(data.activeResourcePack->getFont("Bold"), "PIXELMINER", gui::charSize(*data.vm, 20));
    title->setPosition(sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - title->getGlobalBounds().size.x / 2.f),
                                    static_cast<int>(gui::percent(data.vm->size.y, 15.f))));
    title->setFillColor({240, 240, 240, 255});

    // TITLE
    shadow =
        std::make_unique<sf::Text>(data.activeResourcePack->getFont("Bold"), "PIXELMINER", gui::charSize(*data.vm, 21));
    shadow->setPosition(sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - shadow->getGlobalBounds().size.x / 2.f),
                                     title->getPosition().y + gui::percent(data.vm->size.y, 2.f)));
    shadow->setFillColor({0, 0, 0, 150});

    // BUTTONS
    buttons["Singleplayer"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(title->getPosition().x + title->getGlobalBounds().size.x / 2.f -
                         gui::percent(data.vm->size.x, 45.f) / 2.f,
                     gui::percent(data.vm->size.y, 38.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 45.f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), _("Singleplayer"), data.activeResourcePack->getFont("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Multiplayer"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(title->getPosition().x + title->getGlobalBounds().size.x / 2.f -
                         gui::percent(data.vm->size.x, 45.f) / 2.f,
                     gui::percent(data.vm->size.y, 46.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 45.f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), _("Multiplayer"), data.activeResourcePack->getFont("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Options"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttons["Multiplayer"]->getPosition().x, gui::percent(data.vm->size.y, 54.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 21.8f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), _("Options"), data.activeResourcePack->getFont("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);

    buttons["Quit"] = std::make_unique<gui::TextButton>(
        sf::Vector2f(buttons["Multiplayer"]->getPosition().x + buttons["Multiplayer"]->getSize().x -
                         gui::percent(data.vm->size.x, 21.8f),
                     gui::percent(data.vm->size.y, 54.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 21.8f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), _("Quit"), data.activeResourcePack->getFont("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);
}

MainMenuState::MainMenuState(EngineData &data) : State(data)
{
    initGUI();
}

MainMenuState::~MainMenuState() = default;

void MainMenuState::update(const float &dt)
{
    updateMousePositions();

    for (auto &[key, button] : buttons)
    {
        button->update(mousePosView);

        if (button->isPressed() && data.activeResourcePack->getSound("Click").getStatus() != sf::Sound::Status::Playing)
            data.activeResourcePack->getSound("Click").play();
    }

    if (buttons.at("Singleplayer")->isPressed())
        data.states->push(std::make_shared<WorldSelectionMenuState>(data));

    else if (buttons.at("Multiplayer")->isPressed())
        data.states->push(std::make_shared<ServerSelectionState>(data));

    else if (buttons.at("Quit")->isPressed())
        killSelf();
}

void MainMenuState::render(sf::RenderTarget &target)
{
    target.draw(bgShape);
    target.draw(*shadow);
    target.draw(*title);

    for (auto &[_, button] : buttons)
        button->render(target);
}
