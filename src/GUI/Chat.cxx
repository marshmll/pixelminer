#include "GUI/Chat.hxx"
#include "stdafx.hxx"

using namespace gui;

Chat::Chat(const std::string &this_author, const sf::Vector2f &size, const sf::Vector2f &position, sf::Font &font,
           sf::VideoMode &vm)
    : thisAuthor(sf::String::fromUtf8(this_author.begin(), this_author.end())), font(font), vm(vm), active(false)
{
    float padding = gui::percent(vm.size.x, 1.f);

    chatContainer = std::make_unique<gui::ScrollableContainer>(vm, size, position, 0.f, 5.f);

    input = std::make_unique<gui::Input>(sf::Vector2f(padding, vm.size.y - gui::percent(vm.size.y, 5.f) - padding),
                                         sf::Vector2f(vm.size.x - padding * 2.f, gui::percent(vm.size.y, 5.f)),
                                         sf::Color(0, 0, 0, 80), font, gui::charSize(vm, 120), padding, 1.f,
                                         sf::Color(200, 200, 200, 200), "", true);

    setActive(false);
}

Chat::~Chat() = default;

void Chat::update(const float &dt, const sf::Vector2f &mouse_pos, std::optional<sf::Event> &event,
                  std::optional<sf::Event::MouseWheelScrolled> &mouse_data)
{
    if (active)
    {
        chatContainer->update(dt, mouse_pos, mouse_data);
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
            message->render(target, !active);
    }

    target.setView(target.getDefaultView());
}

void Chat::displayMessage(const std::string &content, const sf::Color &color)
{
    sf::Vector2f next_pos =
        messages.empty()
            ? chatContainer->getPosition()
            : sf::Vector2f((chatContainer->getPosition().x),
                           static_cast<int>(messages.back()->getPosition().y + messages.back()->getSize().y));

    messages.push_back(std::make_unique<Message>(content, next_pos, chatContainer->getSize().x,
                                                 gui::percent(vm.size.x, 1.f), font, gui::charSize(vm, 125), color));

    chatContainer->setMaxScrollDelta(messages.back()->getPosition().y + messages.back()->getSize().y,
                                     gui::percent(chatContainer->getSize().y, 1.f));

    chatContainer->scrollToBottom();
}

void Chat::displayMessage(const std::string &author, const std::string &content, const sf::Color &color)
{
    std::string string;
    string += "[" + author + "] " + content;

    sf::Vector2f next_pos =
        messages.empty()
            ? chatContainer->getPosition()
            : sf::Vector2f((chatContainer->getPosition().x),
                           static_cast<int>(messages.back()->getPosition().y + messages.back()->getSize().y));

    messages.push_back(std::make_unique<Message>(string, next_pos, chatContainer->getSize().x,
                                                 gui::percent(vm.size.x, 1.f), font, gui::charSize(vm, 125), color));

    chatContainer->setMaxScrollDelta(messages.back()->getPosition().y + messages.back()->getSize().y,
                                     gui::percent(chatContainer->getSize().y, 1.f));

    chatContainer->scrollToBottom();
}

void Chat::displayMessage(const sf::String &content, const sf::Color &color)
{
    sf::Vector2f next_pos =
        messages.empty()
            ? chatContainer->getPosition()
            : sf::Vector2f((chatContainer->getPosition().x),
                           static_cast<int>(messages.back()->getPosition().y + messages.back()->getSize().y));

    messages.push_back(std::make_unique<Message>(content, next_pos, chatContainer->getSize().x,
                                                 gui::percent(vm.size.x, 1.f), font, gui::charSize(vm, 125), color));

    chatContainer->setMaxScrollDelta(messages.back()->getPosition().y + messages.back()->getSize().y,
                                     gui::percent(chatContainer->getSize().y, 2.f));

    chatContainer->scrollToBottom();
}

void Chat::displayMessage(const sf::String &author, const sf::String &content, const sf::Color &color)
{
    sf::String string;
    std::string left_bracket = "[";
    std::string right_bracket_colon = "]: ";

    string += sf::String::fromUtf8(left_bracket.begin(), left_bracket.end()) + author +
              sf::String::fromUtf8(right_bracket_colon.begin(), right_bracket_colon.end()) + content;

    sf::Vector2f next_pos =
        messages.empty()
            ? chatContainer->getPosition()
            : sf::Vector2f((chatContainer->getPosition().x),
                           static_cast<int>(messages.back()->getPosition().y + messages.back()->getSize().y));

    messages.push_back(std::make_unique<Message>(string, next_pos, chatContainer->getSize().x,
                                                 gui::percent(vm.size.x, 1.f), font, gui::charSize(vm, 125), color));

    chatContainer->setMaxScrollDelta(messages.back()->getPosition().y + messages.back()->getSize().y,
                                     gui::percent(chatContainer->getSize().y, 2.f));

    chatContainer->scrollToBottom();
}

void Chat::displayGameLog(const std::string &log)
{
    displayMessage(log, sf::Color::Yellow);
}

void Chat::displayGameError(const std::string &log)
{
    displayMessage(log, sf::Color::Red);
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

sf::String Chat::getInputValue()
{
    return input->getValue();
}

void Chat::sendMessageFromInput()
{
    if (input->getValue().getSize() > 0)
    {
        displayMessage(thisAuthor, input->getValue(), sf::Color::White);
        input->clear();
    }
}

void Chat::clearInput()
{
    input->clear();
}
