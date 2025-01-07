#include "States/MainMenuState.hxx"
#include "stdafx.hxx"

void MainMenuState::initGUI()
{
    // BACKGROUND
    if (!bgTexture.loadFromFile("Assets/Images/Backgrounds/stone.png"))
        throw std::runtime_error("[ MainMenuState::initGUI ] -> ERROR LOADING BACKGROUND IMAGE\n");

    bgShape.setPosition({0.f, 0.f});
    bgShape.setSize(sf::Vector2f(data.vm->size.x, data.vm->size.y));
    bgShape.setTexture(&bgTexture);
    bgShape.setFillColor({255, 255, 255, 90});

    // TITLE
    title = std::make_unique<sf::Text>(data.fonts->at("MinecraftBold"), "PIXELMINER", GUI::charSize(*data.vm, 20));
    title->setPosition(sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - title->getGlobalBounds().size.x / 2.f),
                                    static_cast<int>(GUI::percent(data.vm->size.y, 15.f))));
    title->setFillColor({200, 200, 200, 255});
    title->setOutlineThickness(2.f);
    title->setOutlineColor({50, 50, 50, 255});

    // BUTTONS
    buttons["Singleplayer"] = std::make_unique<GUI::TextButton>(
        sf::Vector2f(title->getPosition().x + title->getGlobalBounds().size.x / 2.f -
                         GUI::percent(data.vm->size.x, 45.f) / 2.f,
                     GUI::percent(data.vm->size.y, 38.f)),
        sf::Vector2f(GUI::percent(data.vm->size.x, 45.f), GUI::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), "Singleplayer", data.fonts->at("MinecraftRegular"), GUI::charSize(*data.vm, 83),
        sf::Color::White, 2.f, sf::Color(80, 80, 80, 200));

    buttons["Multiplayer"] = std::make_unique<GUI::TextButton>(
        sf::Vector2f(title->getPosition().x + title->getGlobalBounds().size.x / 2.f -
                         GUI::percent(data.vm->size.x, 45.f) / 2.f,
                     GUI::percent(data.vm->size.y, 46.f)),
        sf::Vector2f(GUI::percent(data.vm->size.x, 45.f), GUI::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), "Multiplayer", data.fonts->at("MinecraftRegular"), GUI::charSize(*data.vm, 83),
        sf::Color::White, 2.f, sf::Color(80, 80, 80, 200));

    buttons["Options"] = std::make_unique<GUI::TextButton>(
        sf::Vector2f(buttons["Multiplayer"]->getPosition().x, GUI::percent(data.vm->size.y, 54.f)),
        sf::Vector2f(GUI::percent(data.vm->size.x, 21.8f), GUI::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), "Options", data.fonts->at("MinecraftRegular"), GUI::charSize(*data.vm, 83),
        sf::Color::White, 2.f, sf::Color(80, 80, 80, 200));

    buttons["Quit"] = std::make_unique<GUI::TextButton>(
        sf::Vector2f(buttons["Multiplayer"]->getPosition().x + buttons["Multiplayer"]->getSize().x -
                         GUI::percent(data.vm->size.x, 21.8f),
                     GUI::percent(data.vm->size.y, 54.f)),
        sf::Vector2f(GUI::percent(data.vm->size.x, 21.8f), GUI::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), "Quit", data.fonts->at("MinecraftRegular"), GUI::charSize(*data.vm, 83),
        sf::Color::White, 2.f, sf::Color(80, 80, 80, 200));
}

MainMenuState::MainMenuState(StateData &data) : State(data)
{
    initGUI();
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::update(const float &dt)
{
    updateMousePositions();

    for (auto &[key, button] : buttons)
        button->update(mousePosView);

    if (buttons.at("Quit")->isPressed())
        killState();
}

void MainMenuState::render(sf::RenderTarget &target)
{
    target.draw(bgShape);
    target.draw(*title);

    for (auto &[key, button] : buttons)
        button->render(target);
}
