#include "stdafx.hxx"
#include "States/Multiplayer/ClientGameState.hxx"

void ClientGameState::initFeedbackScreen()
{
    feedbackBg.setSize(sf::Vector2f(data.vm->size));
    feedbackBg.setPosition(sf::Vector2f(0.f, 0.f));
    feedbackBg.setTexture(&data.activeResourcePack->getTexture("Background"));

    feedbackText = std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"), "Joining server",
                                              gui::charSize(*data.vm, 85));
    feedbackText->setPosition(sf::Vector2f(
        static_cast<int>(gui::percent(data.vm->size.x, 50.f) - feedbackText->getGlobalBounds().size.x / 2.f),
        static_cast<int>(gui::percent(data.vm->size.y, 45.f))));

    feedbackMsg = std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"),
                                             "Attempting to establish connection...", gui::charSize(*data.vm, 85));
    feedbackMsg->setPosition(sf::Vector2f(
        static_cast<int>(gui::percent(data.vm->size.x, 50.f) - feedbackMsg->getGlobalBounds().size.x / 2.f),
        static_cast<int>(gui::percent(data.vm->size.y, 50.f))));

    loaderSprite = std::make_unique<sf::Sprite>(data.activeResourcePack->getTexture("Loader"));
    loaderSprite->setScale(sf::Vector2f(data.scale, data.scale));
    loaderSprite->setPosition(sf::Vector2f(gui::percent(data.vm->size.x, 50.f) - (16.f * data.scale) / 2.f,
                                           gui::percent(data.vm->size.y, 60.f)));

    loaderAnimation = std::make_unique<Animation>(*loaderSprite, data.activeResourcePack->getTexture("Loader"), 100,
                                                  sf::Vector2u(19, 7), sf::Vector2u(0, 0), sf::Vector2u(5, 0), true);
}

ClientGameState::ClientGameState(EngineData &data, const sf::IpAddress &ip, const unsigned short &port)
    : State(data), client(data.uuid), ready(false)
{
    initFeedbackScreen();
    client.connect(ip, port);
}

ClientGameState::~ClientGameState() = default;

void ClientGameState::update(const float &dt)
{
    if (!ready)
    {
        if (!client.isReady())
        {
            updateFeedbackScreen(dt);
            return;
        }
        else if (!client.isConnected())
        {
            replaceSelf(std::make_shared<MessageState>(
                data, "Connection error",
                "The server did not respond after 10 seconds. Check if the "
                "server exists and if it is online, and if your internet connectivity is ok."));
            return;
        }
        else
        {
            ready = true;
        }
    }

    if (!client.isConnected())
    {
        replaceSelf(std::make_shared<MessageState>(data, "Connection error", "Disconnected."));
        return;
    }
}

void ClientGameState::render(sf::RenderTarget &target)
{
    if (!ready)
    {
        renderFeedbackScreen(target);
        return;
    }
}

void ClientGameState::updateFeedbackScreen(const float &dt)
{
    loaderAnimation->play();
}

void ClientGameState::renderFeedbackScreen(sf::RenderTarget &target)
{
    target.draw(feedbackBg);
    target.draw(*feedbackText);
    target.draw(*feedbackMsg);
    target.draw(*loaderSprite);
}