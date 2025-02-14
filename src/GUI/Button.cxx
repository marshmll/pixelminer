#include "GUI/Button.hxx"
#include "stdafx.hxx"

using namespace gui;

Button::Button(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color,
               const float outline_thickness, const sf::Color outline_color)
    : fillColor(fill_color), outlineColor(outline_color), state(ButtonState::Idle)
{
    body.setPosition(position);
    body.setSize(size);
    body.setFillColor(fill_color);
    body.setOutlineThickness(outline_thickness);
    body.setOutlineColor(outline_color);

    brightShadow.setSize(
        sf::Vector2f(body.getSize().x - std::fabs(outline_thickness), body.getSize().y - std::fabs(outline_thickness)));
    brightShadow.setPosition(sf::Vector2f(body.getPosition().x + std::fabs(outline_thickness),
                                          body.getPosition().y + std::fabs(outline_thickness)));
    brightShadow.setFillColor(sf::Color::Transparent);
    brightShadow.setOutlineThickness(outline_thickness);
    brightShadow.setOutlineColor(sf::Color(200, 200, 200, 200));

    darkShadow.setSize(sf::Vector2f(body.getSize().x, body.getSize().y));
    darkShadow.setPosition(sf::Vector2f(body.getPosition().x + std::fabs(outline_thickness),
                                        body.getPosition().y + std::fabs(outline_thickness)));
    darkShadow.setFillColor(sf::Color::Transparent);
    darkShadow.setOutlineThickness(outline_thickness);
    darkShadow.setOutlineColor(sf::Color(50, 50, 50, 140));
}

Button::~Button() = default;

void Button::update(const sf::Vector2f &mouse_pos)
{
    if (state == ButtonState::Disabled)
    {
        body.setFillColor({static_cast<uint8_t>(percent(fillColor.r, 50.f)),
                           static_cast<uint8_t>(percent(fillColor.r, 50.f)),
                           static_cast<uint8_t>(percent(fillColor.r, 50.f)), fillColor.a});
        return;
    }

    state = ButtonState::Idle;
    body.setFillColor({static_cast<uint8_t>(percent(fillColor.r, 80.f)),
                       static_cast<uint8_t>(percent(fillColor.r, 80.f)),
                       static_cast<uint8_t>(percent(fillColor.r, 80.f)), fillColor.a});

    if (body.getGlobalBounds().contains(mouse_pos))
    {
        state = ButtonState::Hover;
        body.setFillColor({static_cast<uint8_t>(percent(fillColor.r, 70.f)),
                           static_cast<uint8_t>(percent(fillColor.r, 70.f)),
                           static_cast<uint8_t>(percent(fillColor.r, 70.f)), fillColor.a});

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            state = ButtonState::Active;
            body.setFillColor(fillColor);
        }
    }
}

void Button::render(sf::RenderTarget &target)
{
    target.draw(darkShadow);
    target.draw(body);
    target.draw(brightShadow);
}

const bool Button::isPressed() const
{
    return state == ButtonState::Active;
}

const sf::Vector2f Button::getPosition() const
{
    return body.getPosition();
}

const sf::Vector2f Button::getSize() const
{
    return body.getSize();
}

void Button::setState(const ButtonState &state)
{
    this->state = state;
}