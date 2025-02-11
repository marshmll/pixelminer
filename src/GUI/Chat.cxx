#include "GUI/Chat.hxx"
#include "stdafx.hxx"

using namespace gui;

Chat::Chat(const std::string &this_author, const sf::Vector2f &size, const sf::Vector2f &position, sf::Font &font,
           sf::VideoMode &vm)
    : thisAuthor(this_author), font(font), vm(vm), active(false)
{
    float padding = gui::percent(vm.size.x, 1.f);

    chatContainer = std::make_unique<gui::ScrollableContainer>(vm, size, position, 0.f, 2.f);

    input = std::make_unique<gui::Input>(sf::Vector2f(padding, vm.size.y - gui::percent(vm.size.y, 5.f) - padding),
                                         sf::Vector2f(vm.size.x - padding * 2.f, gui::percent(vm.size.y, 5.f)),
                                         sf::Color(0, 0, 0, 80), font, gui::charSize(vm, 120), padding, 1.f,
                                         sf::Color(200, 200, 200, 200));

    setActive(false);
}

Chat::~Chat()
{
}

void Chat::update(const float &dt, const sf::Vector2f &mouse_pos, std::optional<sf::Event> &event,
                  sf::Event::MouseWheelScrolled &mouse_data)
{
    if (active)
    {
        chatContainer->update(dt, mouse_pos, event, mouse_data);
        input->update(dt, mouse_pos, event);
    }

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
    if (active)
    {
        chatContainer->render(target);
        input->render(target);
    }

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

void Chat::displayMessage(const std::string &author, const std::string &content, const sf::Color &color)
{
    sf::Vector2f next_pos = messages.empty()
                                ? chatContainer->getPosition()
                                : sf::Vector2f(chatContainer->getPosition().x, messages.back()->getPosition().y +
                                                                                   messages.back()->getSize().y -
                                                                                   gui::percent(vm.size.y, 1.f));

    messages.push_back(std::make_unique<Message>(author, content, next_pos, chatContainer->getSize().x,
                                                 gui::percent(vm.size.x, 1.f), font, gui::charSize(vm, 125), color));

    chatContainer->setMaxScrollDelta(messages.back()->getPosition().y + messages.back()->getSize().y,
                                     gui::percent(chatContainer->getSize().y, 2.f));

    chatContainer->scrollToBottom();
}

void Chat::displayGameLog(const std::string &log)
{
    displayMessage("", log, sf::Color::Yellow);
}

void Chat::displayGameError(const std::string &log)
{
    displayMessage("", log, sf::Color::Red);
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

std::string Chat::getInputValue()
{
    return input->getValue();
}

void Chat::sendMessageFromInput()
{
    if (input->getValue().size() > 0)
    {
        displayMessage(thisAuthor, input->getValue());
        input->clear();
    }
}

void Chat::clearInput()
{
    input->clear();
}