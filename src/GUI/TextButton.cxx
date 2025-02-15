#include "GUI/TextButton.hxx"
#include "stdafx.hxx"

using namespace gui;

TextButton::TextButton(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color,
                       const std::string str, const sf::Font &font, const unsigned int char_size,
                       const sf::Color text_color, const float outline_thickness, const sf::Color outline_color)
    : Button(position, size, fill_color, outline_thickness, outline_color), textColor(text_color)
{
    text = std::make_unique<sf::Text>(font, sf::String::fromUtf8(str.begin(), str.end()), char_size);
    text->setFillColor(text_color);
    text->setPosition(sf::Vector2f(
        static_cast<int>(body.getPosition().x + body.getSize().x / 2.f - text->getGlobalBounds().size.x / 2.f),
        static_cast<int>(body.getPosition().y + body.getSize().y / 2.f - text->getGlobalBounds().size.y / 2.f -
                         char_size / 6.f)));

    textShadow = std::make_unique<sf::Text>(font, sf::String::fromUtf8(str.begin(), str.end()), char_size);
    textShadow->setFillColor(sf::Color(0, 0, 0, 170));
    textShadow->setPosition(sf::Vector2f(
        static_cast<int>(body.getPosition().x + body.getSize().x / 2.f - text->getGlobalBounds().size.x / 2.f - 2.f),
        static_cast<int>(body.getPosition().y + body.getSize().y / 2.f - text->getGlobalBounds().size.y / 2.f -
                         char_size / 6.f - 2.f)));
}

TextButton::~TextButton()
{}

void TextButton::update(const sf::Vector2f &mouse_pos)
{
    if (state == ButtonState::Disabled)
    {
        body.setFillColor({static_cast<uint8_t>(percent(fillColor.r, 50.f)),
                           static_cast<uint8_t>(percent(fillColor.r, 50.f)),
                           static_cast<uint8_t>(percent(fillColor.r, 50.f)), fillColor.a});

        text->setFillColor({static_cast<uint8_t>(percent(textColor.r, 50.f)),
                            static_cast<uint8_t>(percent(textColor.r, 50.f)),
                            static_cast<uint8_t>(percent(textColor.r, 50.f)), textColor.a});
        return;
    }

    state = ButtonState::Idle;
    body.setFillColor({static_cast<uint8_t>(percent(fillColor.r, 80.f)),
                       static_cast<uint8_t>(percent(fillColor.r, 80.f)),
                       static_cast<uint8_t>(percent(fillColor.r, 80.f)), fillColor.a});

    text->setFillColor({static_cast<uint8_t>(percent(textColor.r, 90.f)),
                        static_cast<uint8_t>(percent(textColor.r, 90.f)),
                        static_cast<uint8_t>(percent(textColor.r, 90.f)), textColor.a});

    if (body.getGlobalBounds().contains(mouse_pos))
    {
        state = ButtonState::Hover;
        body.setFillColor(fillColor);
        text->setFillColor(textColor);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            state = ButtonState::Active;
            body.setFillColor({static_cast<uint8_t>(percent(fillColor.r, 70.f)),
                               static_cast<uint8_t>(percent(fillColor.r, 70.f)),
                               static_cast<uint8_t>(percent(fillColor.r, 70.f)), fillColor.a});

            text->setFillColor({static_cast<uint8_t>(percent(textColor.r, 70.f)),
                                static_cast<uint8_t>(percent(textColor.r, 70.f)),
                                static_cast<uint8_t>(percent(textColor.r, 70.f)), textColor.a});
        }
    }
}

void TextButton::render(sf::RenderTarget &target)
{
    if (state != ButtonState::Disabled)
        target.draw(darkShadow);
    target.draw(body);
    if (state != ButtonState::Disabled)
        target.draw(brightShadow);
    target.draw(*textShadow);
    target.draw(*text);
}