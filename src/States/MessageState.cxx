#include "stdafx.hxx"
#include "States/MessageState.hxx"

MessageState::MessageState(EngineData &data, const std::string &message, const std::string &description) : State(data)
{
    bg.setSize(sf::Vector2f(data.vm->size));
    bg.setPosition(sf::Vector2f(0.f, 0.f));
    bg.setTexture(&data.activeResourcePack->getTexture("Background"));

    messageText =
        std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"),
                                   sf::String::fromUtf8(message.begin(), message.end()), gui::charSize(*data.vm, 85));

    messageText->setPosition(
        sf::Vector2f(gui::percent(data.vm->size.x, 50.f) - messageText->getGlobalBounds().size.x / 2.f,
                     gui::percent(data.vm->size.y, 40.f)));

    messageText->setFillColor(sf::Color(200, 200, 200, 255));

    descriptionText = std::make_unique<sf::Text>(data.activeResourcePack->getFont("Regular"),
                                                 sf::String::fromUtf8(description.begin(), description.end()),
                                                 gui::charSize(*data.vm, 85));

    descriptionText->setPosition(sf::Vector2f(0.f, 0.f));

    for (int i = 0; i < descriptionText->getString().getSize(); i++)
    {
        if ((descriptionText->findCharacterPos(i).x - descriptionText->getPosition().x) >=
            gui::percent(data.vm->size.x, 80.f))
        {
            sf::String str = descriptionText->getString();
            int j = i;

            while (j > 0 && str[j] != ' ')
                j--;

            str.replace(j, 1, "\n");
            descriptionText->setString(str);
        }
    }

    descriptionText->setPosition(
        sf::Vector2f(gui::percent(data.vm->size.x, 50.f) - descriptionText->getGlobalBounds().size.x / 2.f,
                     gui::percent(data.vm->size.y, 50.f)));

    descriptionText->setFillColor(sf::Color::White);

    backButton = std::make_unique<gui::TextButton>(
        sf::Vector2f(descriptionText->getPosition().x + descriptionText->getGlobalBounds().size.x / 2.f -
                         gui::percent(data.vm->size.x, 45.f) / 2.f,
                     gui::percent(data.vm->size.y, 80.f)),
        sf::Vector2f(gui::percent(data.vm->size.x, 45.f), gui::percent(data.vm->size.y, 6.f)),
        sf::Color(200, 200, 200, 200), _("Back"), data.activeResourcePack->getFont("Regular"),
        gui::charSize(*data.vm, 95), sf::Color::White, 2.f, sf::Color::Black);
}

MessageState::~MessageState() = default;

void MessageState::update(const float &dt)
{
    updateMousePositions();

    backButton->update(mousePosView);

    if (backButton->isPressed())
    {
        data.activeResourcePack->getSound("Click").play();
        killSelf();
    }
}

void MessageState::render(sf::RenderTarget &target)
{
    target.draw(bg);
    target.draw(*messageText);
    target.draw(*descriptionText);
    backButton->render(target);
}