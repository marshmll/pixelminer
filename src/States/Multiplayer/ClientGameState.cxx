#include "stdafx.hxx"
#include "States/Multiplayer/ClientGameState.hxx"

void ClientGameState::initFeedbackScreen()
{
    feedbackBg.setSize(sf::Vector2f(data.vm->size));
    feedbackBg.setPosition(sf::Vector2f(0.f, 0.f));
    feedbackBg.setTexture(&data.activeResourcePack->getTexture("Background"));

    std::string str = _("Joining server");

    feedbackText =
        std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"),
                                   sf::String::fromUtf8(str.begin(), str.end()), gui::charSize(*data.vm, 85));
    feedbackText->setPosition(sf::Vector2f(
        static_cast<int>(gui::percent(data.vm->size.x, 50.f) - feedbackText->getGlobalBounds().size.x / 2.f),
        static_cast<int>(gui::percent(data.vm->size.y, 45.f))));

    str = _("Attempting to establish connection...");

    feedbackMsg = std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"),
                                             sf::String::fromUtf8(str.begin(), str.end()), gui::charSize(*data.vm, 85));
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
    if (client.getStatus() == ClientStatus::SockError)
    {
        replaceSelf(std::make_shared<MessageState>(data, _("Socket error"),
                                                   _("Could not bind the client UDP socket to a port. This may be a "
                                                     "system error or a invalid configuration of the socket.")));
    }
    else
    {
        initFeedbackScreen();
        client.connect(ip, port);
    }
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
        else if (client.getStatus() == ClientStatus::TimedOut)
        {
            replaceSelf(std::make_shared<MessageState>(
                data, _("Connection error"),
                _("The server did not respond after 10 seconds. Check if the server exists and if it is online, and if "
                  "your internet connectivity is ok.")));
            return;
        }
        else if (client.getStatus() == ClientStatus::Refused)
        {
            replaceSelf(
                std::make_shared<MessageState>(data, _("Connection error"),
                                               _("The server refused to connect with this client. This might be "
                                                 "because of a attempt to a self-connection or a server ban.")));
            return;
        }
        else
        {
            ready = true;
        }
    }

    if (client.getStatus() == ClientStatus::Disconnected)
    {
        replaceSelf(std::make_shared<MessageState>(data, _("Connection error"), _("Disconnected.")));
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
