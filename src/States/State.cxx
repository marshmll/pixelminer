#include "States/State.hxx"
#include "stdafx.hxx"

State::State(EngineData &data)
    : data(data), dead(false), replaced(false), renderTexture(data.vm->size), renderSprite(renderTexture.getTexture())
{
    keyClock.restart();
    mouseClock.restart();
}

State::~State()
{}

void State::update(const float &dt)
{}

void State::render(sf::RenderTarget &target)
{}

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

const bool State::mouseButtonPressedWithin(const std::int32_t &milliseconds, const sf::Mouse::Button &button)
{
    if (mouseClock.getElapsedTime().asMilliseconds() >= milliseconds && sf::Mouse::isButtonPressed(button))
    {
        mouseClock.restart();
        return true;
    }

    return false;
}

void State::killSelf()
{
    dead = true;
}

void State::replaceSelf(std::shared_ptr<State> state)
{
    replacerState = state;
    replaced = true;
}

std::shared_ptr<State> &State::getReplacerState()
{
    return replacerState;
}

const bool &State::isDead() const
{
    return dead;
};

const bool &State::wasReplaced() const
{
    return replaced;
}