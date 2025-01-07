#include "GUI/Button.hxx"
#include "stdafx.hxx"

using namespace GUI;

Button::Button(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color,
               const float outline_thickness, const sf::Color outline_color)
    : fillColor(fill_color), outlineColor(outline_color)
{
    body.setPosition(position);
    body.setSize(size);
    body.setFillColor(fill_color);
    body.setOutlineThickness(outline_thickness);
    body.setOutlineColor(outline_color);
}

Button::~Button()
{
}

void Button::update(const sf::Vector2f &mouse_pos)
{
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
    target.draw(body);
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