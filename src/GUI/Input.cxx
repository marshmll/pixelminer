#include "GUI/Input.hxx"
#include "stdafx.hxx"

using namespace gui;

Input::Input(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &body_color, sf::Font &font,
             const unsigned int &char_size, const float &padding, const float &outline_thickness,
             const sf::Color outline_color)
    : charSize(char_size), padding(padding), cursorTimer(0.f), cursorTimerMax(.2f), repeat(false)
{
    body.setSize(size);
    body.setPosition(position);
    body.setFillColor(body_color);
    body.setOutlineThickness(outline_thickness);
    body.setOutlineColor(outline_color);

    text = std::make_unique<sf::Text>(font, "Teste", char_size);
    text->setPosition(sf::Vector2f(static_cast<int>(body.getPosition().x + padding),
                                   static_cast<int>(body.getPosition().y + padding - char_size / 5.f)));

    blinkerCursor.setSize(sf::Vector2f(char_size / 2.f, text->getLetterSpacing()));
    blinkerCursor.setPosition(sf::Vector2f(text->getPosition().x + text->getGlobalBounds().size.x,
                                           body.getPosition().y + body.getSize().y - padding));
    blinkerCursor.setFillColor(sf::Color::White);

    keyTimer.restart();
}

Input::~Input()
{
}

void Input::toggleCursorVisibility()
{
    cursorTimer = 0.f;
    blinkerCursor.setFillColor(blinkerCursor.getFillColor() == sf::Color::White ? sf::Color::Transparent
                                                                                : sf::Color::White);
}

void Input::handleKeyPress(char32_t c)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
    {
        if (std::isalpha(c))
            c = toupper(c);
        else if (c == '1')
            c = '!';
        else if (c == '2')
            c = '@';
        else if (c == '3')
            c = '#';
        else if (c == '4')
            c = '$';
        else if (c == '5')
            c = '%';
        else if (c == '7')
            c = '&';
        else if (c == '8')
            c = '*';
        else if (c == '9')
            c = '(';
        else if (c == '0')
            c = ')';
        else if (c == ';')
            c = ':';
        else if (c == ']')
            c = '}';
        else if (c == '[')
            c = '{';
    }

    std::string currStr = text->getString();
    currStr.push_back(c);
    text->setString(currStr);
}

void Input::handleBackspace()
{
    std::string currStr = text->getString();
    if (!currStr.empty())
    {
        currStr.pop_back();
        text->setString(currStr);
    }
}

void Input::update(const float &dt, sf::Vector2f mouse_pos, std::optional<sf::Event> &event)
{
    cursorTimer += dt;
    if (cursorTimer >= cursorTimerMax)
        toggleCursorVisibility();

    blinkerCursor.setPosition(sf::Vector2f(text->getPosition().x + text->getGlobalBounds().size.x,
                                           body.getPosition().y + body.getSize().y - padding));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace))
    {
        if (lastKeyPressed == sf::Keyboard::Key::Backspace && keyTimer.getElapsedTime().asSeconds() > .5f)
        {
            repeat = true;
            handleBackspace();
        }
        else if (lastKeyPressed != sf::Keyboard::Key::Backspace)
        {
            repeat = false;
            handleBackspace();
            keyTimer.restart();
        }
        else if (repeat)
        {
            handleBackspace();
        }

        lastKeyPressed = sf::Keyboard::Key::Backspace;
        return;
    }
    else
    {
        for (const auto &[key, character] : keyMap)
        {
            if (sf::Keyboard::isKeyPressed(key))
            {
                if (lastKeyPressed == key && keyTimer.getElapsedTime().asSeconds() > .5f)
                {
                    repeat = true;
                    handleKeyPress(character);
                }
                else if (lastKeyPressed != key)
                {
                    repeat = false;
                    handleKeyPress(character);
                    keyTimer.restart();
                }
                else if (repeat)
                {
                    handleKeyPress(character);
                }

                lastKeyPressed = key;
                return;
            }
        }
    }

    lastKeyPressed = sf::Keyboard::Key::Unknown;
    keyTimer.restart();
}

void Input::render(sf::RenderTarget &target)
{
    target.draw(body);
    target.draw(*text);
    target.draw(blinkerCursor);
}
