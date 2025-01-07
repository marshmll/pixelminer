#include "States/MainMenuState.hxx"
#include "stdafx.hxx"

void MainMenuState::initGUI()
{
    if (!bgTexture.loadFromFile("Assets/Images/Backgrounds/background.png"))
    {
        std::cerr << "[ MainMenuState::initGUI ] -> ERROR LOADING BACKGROUND IMAGE" << "\n";

        // TODO: Handle memory safe exit.
    };

    bgShape.setPosition({0.f, 0.f});

    bgShape.setSize(sf::Vector2f(data.vm->size.x, data.vm->size.y));
    bgShape.setTexture(&bgTexture);
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
}

void MainMenuState::render(sf::RenderTarget &target)
{
    target.draw(bgShape);
}