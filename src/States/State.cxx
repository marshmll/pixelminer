#include "States/State.hxx"
#include "stdafx.hxx"

State::State(EngineData &data)
    : data(data), dead(false), replaced(false), restartAll(false), renderTexture(data.vm->size),
      renderSprite(renderTexture.getTexture())
{
    keyClock.restart();
    mouseClock.restart();
}

State::~State() = default;

void State::update(const float &dt)
{}

void State::render(sf::RenderTarget &target)
{}

void State::updateMousePositions(std::optional<sf::View> view)
{
    mousePosScreen = sf::Mouse::getPosition();
    mousePosWindow = sf::Mouse::getPosition(*data.window);

    if (view)
        data.window->setView(view.value());

    mousePosView = data.window->mapPixelToCoords(mousePosWindow);
    mousePosGrid = sf::Vector2i(static_cast<int>(mousePosView.x / (data.gridSize * data.scale)),
                                static_cast<int>(mousePosView.y / (data.gridSize * data.scale)));

    if (view)
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

void State::replaceSelf(const std::shared_ptr<State> &state)
{
    replacerState = std::move(state);
    replaced = true;
}

void State::restartStates()
{
    restartAll = true;
}

std::shared_ptr<State> &State::getReplacerState()
{
    return replacerState;
}

const bool &State::isDead() const
{
    return dead;
}

const bool &State::wasReplaced() const
{
    return replaced;
}

const bool &State::askedToRestartAllStates() const
{
    return restartAll;
}
