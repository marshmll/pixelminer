#include "States/State.hxx"
#include "stdafx.hxx"

State::State(EngineData &data) : data(data), dead(false), renderTexture(data.vm->size), renderSprite(renderTexture.getTexture())
{
    keyClock.restart();
}

State::~State()
{
}

void State::killState()
{
    dead = true;
}

const bool &State::isDead() const
{
    return dead;
};

void State::updateMousePositions(std::optional<sf::View> view)
{
    mousePosScreen = sf::Mouse::getPosition();
    mousePosWindow = sf::Mouse::getPosition(*data.window);

    if (view.has_value())
        data.window->setView(view.value());

    mousePosView = data.window->mapPixelToCoords(mousePosWindow);
    mousePosGrid = {static_cast<int>(mousePosView.x / data.gridSize), static_cast<int>(mousePosView.y / data.gridSize)};
    if (view.has_value())
        data.window->setView(data.window->getDefaultView());
}

const bool State::keyPressedWithin(const std::int32_t &milliseconds, const sf::Keyboard::Key &key)
{
    if (keyClock.getElapsedTime().asMilliseconds() >= milliseconds && sf::Keyboard::isKeyPressed(key))
    {
        keyClock.restart();
        return true;
    }

    return false;
}