#include "GUI/Input.hxx"
#include "stdafx.hxx"

using namespace gui;

Input::Input(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &body_color, sf::Font &font,
             const unsigned int &char_size, const float &padding, const float &outline_thickness,
             const sf::Color outline_color, const std::string label)
    : charSize(char_size), padding(padding), cursorTimer(0.f), cursorTimerMax(.2f), cursorIndex(0), repeat(false)
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

    value = std::make_unique<sf::Text>(font, "", char_size);
    value->setPosition(sf::Vector2f(static_cast<int>(body.getPosition().x + padding),
                                    static_cast<int>(body.getPosition().y + padding - char_size / 3.f)));
    value->setFillColor(sf::Color::White);

    blinkerCursor.setSize(sf::Vector2f(char_size / 2.f, value->getLetterSpacing()));
    blinkerCursor.setPosition(sf::Vector2f(value->getPosition().x + value->getGlobalBounds().size.x,
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
        switch (c)
        {
        case '1':
            c = '!';
            break;
        case '2':
            c = '@';
            break;
        case '3':
            c = '#';
            break;
        case '4':
            c = '$';
            break;
        case '5':
            c = '%';
            break;
        case '6':
            c = '^';
            break;
        case '7':
            c = '&';
            break;
        case '8':
            c = '*';
            break;
        case '9':
            c = '(';
            break;
        case '0':
            c = ')';
            break;
        case '-':
            c = '_';
            break;
        case '=':
            c = '+';
            break;
        case '[':
            c = '{';
            break;
        case ']':
            c = '}';
            break;
        case '\\':
            c = '|';
            break;
        case ';':
            c = ':';
            break;
        case '\'':
            c = '"';
            break;
        case ',':
            c = '<';
            break;
        case '.':
            c = '>';
            break;
        case '/':
            c = '?';
            break;
        default:
            if (std::isalpha(c))
                c = toupper(c);
            break;
        }
    }

    sf::String currStr = value->getString();
    currStr.insert(cursorIndex, c);
    value->setString(currStr);
    cursorIndex++;
}

void Input::handleBackspace()
{
    sf::String currStr = value->getString();
    if (cursorIndex > 0)
    {
        currStr.erase(cursorIndex - 1, 1);
        value->setString(currStr);
        --cursorIndex;
    }
}

void Input::handleTab()
{
    sf::String currStr = value->getString();
    currStr.insert(cursorIndex, "    ");
    value->setString(currStr);
    cursorIndex += 4;
}

void Input::handleEnter()
{
    sf::String currStr = value->getString();
    currStr.insert(cursorIndex, "\n");
    value->setString(currStr);
    ++cursorIndex;
}

void Input::handleArrowKey(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::Right && cursorIndex < value->getString().getSize())
        ++cursorIndex;
    if (key == sf::Keyboard::Key::Left && cursorIndex > 0)
        --cursorIndex;
}

void Input::handleSpecialKey(sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Key::Escape:
        // Handle escape key
        break;
    case sf::Keyboard::Key::Space:
        handleKeyPress(' ');
        break;
    case sf::Keyboard::Key::Left:
    case sf::Keyboard::Key::Right:
        handleArrowKey(key);
        break;
    default:
        break;
    }
}

void Input::update(const float &dt, sf::Vector2f mouse_pos, std::optional<sf::Event> &event)
{
    cursorTimer += dt;
    if (cursorTimer >= cursorTimerMax)
        toggleCursorVisibility();

    // Update cursor position using findCharacterPos
    if (cursorIndex <= value->getString().getSize())
    {
        sf::Vector2f cursorPos = value->findCharacterPos(cursorIndex);
        blinkerCursor.setPosition(sf::Vector2f(cursorPos.x, body.getPosition().y + body.getSize().y - padding));
    }

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
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab))
    {
        if (lastKeyPressed == sf::Keyboard::Key::Tab && keyTimer.getElapsedTime().asSeconds() > .5f)
        {
            repeat = true;
            handleTab();
        }
        else if (lastKeyPressed != sf::Keyboard::Key::Tab)
        {
            repeat = false;
            handleTab();
            keyTimer.restart();
        }
        else if (repeat)
        {
            handleTab();
        }

        lastKeyPressed = sf::Keyboard::Key::Tab;
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

        // Handle special keys
        for (const auto &key : specialKeys)
        {
            if (sf::Keyboard::isKeyPressed(key))
            {
                if (lastKeyPressed == key && keyTimer.getElapsedTime().asSeconds() > .5f)
                {
                    repeat = true;
                    handleSpecialKey(key);
                }
                else if (lastKeyPressed != key)
                {
                    repeat = false;
                    handleSpecialKey(key);
                    keyTimer.restart();
                }
                else if (repeat)
                {
                    handleSpecialKey(key);
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
    target.draw(*value);
    target.draw(*label);
    target.draw(blinkerCursor);
}

const std::string Input::getValue() const
{
    return value->getString();
}