#include "GUI/Chat.hxx"
#include "stdafx.hxx"

using namespace gui;

Chat::Chat(const sf::Vector2f &size, const sf::Vector2f &position, sf::Font &font, sf::VideoMode &vm)
    : font(font), vm(vm), active(false)
{
    chatContainer = std::make_unique<gui::ScrollableContainer>(vm, size, position, 0.f, 2.f);
    setActive(false);

    displayMessage("marshmll", "hello, world!");
}

Chat::~Chat()
{
}

void Chat::update(const float &dt, const sf::Vector2f &mouse_pos, std::optional<sf::Event> &event,
                  sf::Event::MouseWheelScrolled &mouse_data)
{
    if (active)
        chatContainer->update(dt, mouse_pos, event, mouse_data);

    for (auto &message : messages)
    {
        if (message)
        {
            if (message->isVisible())
                message->update(dt, !active);
        }
    }
}

void Chat::render(sf::RenderTarget &target)
{
    chatContainer->render(target);

    target.setView(chatContainer->getView());

    for (auto &message : messages)
    {
        if (message)
        {
            message->render(target, !active);
        }
    }

    target.setView(target.getDefaultView());
}

void Chat::displayMessage(const std::string &author, const std::string &content)
{
    sf::Vector2f next_pos = messages.empty()
                                ? chatContainer->getPosition()
                                : sf::Vector2f(chatContainer->getPosition().x, messages.back()->getPosition().y +
                                                                                   messages.back()->getSize().y -
                                                                                   gui::percent(vm.size.y, 1.f));

    messages.push_back(std::make_unique<Message>(author, content, next_pos, chatContainer->getSize().x,
                                                 gui::percent(vm.size.x, 1.f), font, gui::charSize(vm, 125)));

    chatContainer->setMaxScrollDelta(messages.back()->getPosition().y + messages.back()->getSize().y,
                                     gui::percent(chatContainer->getSize().y, 2.f));

    chatContainer->scrollToBottom();
}

const bool &Chat::isActive() const
{
    return active;
}

void Chat::setActive(const bool &active)
{
    this->active = active;
    chatContainer->setScrollBarVisibility(active);

    if (!active)
        chatContainer->scrollToBottom();
}