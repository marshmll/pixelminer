#include "States/Substate.hxx"

Substate::Substate(EngineData &data) : data(data), dead(false), restartAll(false)
{}

Substate::~Substate() = default;

void Substate::updateMousePositions(std::optional<sf::View> view)
{
    mousePosScreen = sf::Mouse::getPosition();
    mousePosWindow = sf::Mouse::getPosition(*data.window);

    if (view)
        data.window->setView(view.value());

    mousePosView = data.window->mapPixelToCoords(mousePosWindow);
    mousePosGrid = sf::Vector2i(static_cast<int>(mousePosView.x / (data.gridSize * *data.scale)),
                                static_cast<int>(mousePosView.y / (data.gridSize * *data.scale)));

    if (view)
        data.window->setView(data.window->getDefaultView());
}

const bool Substate::keyPressedWithin(const std::int32_t &milliseconds, const sf::Keyboard::Key &key)
{
    if (keyClock.getElapsedTime().asMilliseconds() >= milliseconds && sf::Keyboard::isKeyPressed(key))
    {
        keyClock.restart();
        return true;
    }

    return false;
}

const bool Substate::mouseButtonPressedWithin(const std::int32_t &milliseconds, const sf::Mouse::Button &button)
{
    if (mouseClock.getElapsedTime().asMilliseconds() >= milliseconds && sf::Mouse::isButtonPressed(button))
    {
        mouseClock.restart();
        return true;
    }

    return false;
}

void Substate::killSelf()
{
    dead = true;
}

void Substate::restartStates()
{
    restartAll = true;
}

const bool &Substate::isDead() const
{
    return dead;
}

const bool &Substate::askedToRestartAllStates() const
{
    return restartAll;
}
