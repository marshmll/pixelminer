#include "GUI/Input.hxx"
#include "stdafx.hxx"

using namespace gui;

Input::Input(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &body_color, sf::Font &font,
             const unsigned int &char_size, const float &padding, const float &outline_thickness,
             const sf::Color outline_color, const std::string label, const bool &focus)
    : charSize(char_size), padding(padding), cursorTimer(0.f), cursorTimerMax(.2f), cursorIndex(0), repeat(false),
      focus(focus)
{
    body.setSize(size);
    body.setPosition(position);
    body.setFillColor(body_color);
    body.setOutlineThickness(outline_thickness);
    body.setOutlineColor(outline_color);

    this->label = std::make_unique<sf::Text>(font, label, char_size);
    this->label->setPosition(
        sf::Vector2f(static_cast<int>(body.getPosition().x),
                     static_cast<int>(body.getPosition().y - this->label->getGlobalBounds().size.y - char_size)));
    this->label->setFillColor(sf::Color(255, 255, 255, 200));

    text = std::make_unique<sf::Text>(font, "", char_size);
    text->setPosition(sf::Vector2f(static_cast<int>(body.getPosition().x + padding),
                                   static_cast<int>(body.getPosition().y + padding - char_size / 3.f)));
    text->setFillColor(sf::Color::White);

    blinkerCursor.setSize(sf::Vector2f(char_size / 2.f, text->getLetterSpacing()));
    blinkerCursor.setPosition(sf::Vector2f(text->getPosition().x + text->getGlobalBounds().size.x,
                                           body.getPosition().y + body.getSize().y - padding));
    blinkerCursor.setFillColor(sf::Color::White);

    keyTimer.restart();
    setFocus(focus);
}

Input::~Input() = default;

void Input::toggleCursorVisibility()
{
    cursorTimer = 0.f;
    blinkerCursor.setFillColor(blinkerCursor.getFillColor() == sf::Color::White ? sf::Color::Transparent
                                                                                : sf::Color::White);
}

void Input::handleKeyPress(char32_t c)
{
    if (std::iscntrl(c))
        return;

    sf::String currStr = text->getString();
    currStr.insert(cursorIndex, c);
    text->setString(currStr);
    cursorIndex++;
}

void Input::handleBackspace()
{
    sf::String currStr = text->getString();
    if (cursorIndex > 0)
    {
        currStr.erase(cursorIndex - 1, 1);
        text->setString(currStr);
        --cursorIndex;
    }
}

void Input::handleArrowKey(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::Right && cursorIndex < text->getString().getSize())
        ++cursorIndex;
    if (key == sf::Keyboard::Key::Left && cursorIndex > 0)
        --cursorIndex;
}

void Input::update(const float &dt, sf::Vector2f mouse_pos, std::optional<sf::Event> &event)
{
    if (!focus)
    {
        if (this->body.getGlobalBounds().contains(mouse_pos))
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                setFocus(true);
        }

        return;
    }
    else if (!this->body.getGlobalBounds().contains(mouse_pos))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            setFocus(false);
    }

    cursorTimer += dt;
    if (cursorTimer >= cursorTimerMax)
        toggleCursorVisibility();

    // Update cursor position using findCharacterPos
    if (cursorIndex <= text->getString().getSize())
    {
        sf::Vector2f cursorPos = text->findCharacterPos(cursorIndex);
        blinkerCursor.setPosition(sf::Vector2f(cursorPos.x, body.getPosition().y + body.getSize().y - padding));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace))
    {
        if (lastKeyPressed == '\b' && keyTimer.getElapsedTime().asSeconds() > .5f)
        {
            repeat = true;
            handleBackspace();
        }
        else if (lastKeyPressed != '\b')
        {
            repeat = false;
            handleBackspace();
            keyTimer.restart();
        }
        else if (repeat)
        {
            handleBackspace();
        }

        lastKeyPressed = '\b';
        return;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        if (lastKeyPressed == -2 && keyTimer.getElapsedTime().asSeconds() > .5f)
        {
            repeat = true;
            handleArrowKey(sf::Keyboard::Key::Left);
        }
        else if (lastKeyPressed != -2)
        {
            repeat = false;
            handleArrowKey(sf::Keyboard::Key::Left);
            keyTimer.restart();
        }
        else if (repeat)
        {
            handleArrowKey(sf::Keyboard::Key::Left);
        }

        lastKeyPressed = -2;
        return;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        if (lastKeyPressed == -3 && keyTimer.getElapsedTime().asSeconds() > .5f)
        {
            repeat = true;
            handleArrowKey(sf::Keyboard::Key::Right);
        }
        else if (lastKeyPressed != -3)
        {
            repeat = false;
            handleArrowKey(sf::Keyboard::Key::Right);
            keyTimer.restart();
        }
        else if (repeat)
        {
            handleArrowKey(sf::Keyboard::Key::Right);
        }

        lastKeyPressed = -3;
        return;
    }
    else if (const auto *text_entered = event->getIf<sf::Event::TextEntered>())
    {
        sf::String currStr = text->getString();
        char32_t unicode = text_entered->unicode;

        if (lastKeyPressed == unicode && keyTimer.getElapsedTime().asSeconds() > .5f)
        {
            repeat = true;
            handleKeyPress(unicode);
        }
        else if (lastKeyPressed != unicode)
        {
            repeat = false;
            handleKeyPress(unicode);
            keyTimer.restart();
        }
        else if (repeat)
        {
            handleKeyPress(unicode);
        }

        lastKeyPressed = unicode;
        return;
    }

    lastKeyPressed = -1;
    keyTimer.restart();
}

void Input::render(sf::RenderTarget &target)
{
    target.draw(body);
    target.draw(*text);
    target.draw(*label);

    if (focus)
        target.draw(blinkerCursor);
}

const std::string Input::getValue() const
{
    return text->getString();
}

const bool &Input::getFocus() const
{
    return focus;
}

void Input::setFocus(const bool &focus)
{
    this->focus = focus;
    if (focus)
        body.setOutlineThickness(2.f);
    else
        body.setOutlineThickness(0.f);
}

void Input::clear()
{
    text->setString("");
    cursorIndex = 0;
}