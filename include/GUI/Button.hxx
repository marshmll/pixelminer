#pragma once

#include "GUI/Utils.hxx"

namespace GUI
{
enum ButtonState
{
    Idle = 0,
    Hover,
    Active,
};

class Button
{
  protected:
    sf::RectangleShape body;
    sf::Color fillColor;
    sf::Color outlineColor;
    ButtonState state;

  public:
    Button(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color,
           const float outline_thickness = 0.f, const sf::Color outline_color = sf::Color::Transparent);

    virtual ~Button();

    virtual void update(const sf::Vector2f &mouse_pos);

    virtual void render(sf::RenderTarget &target);

    const bool isPressed() const;

    const sf::Vector2f getPosition() const;

    const sf::Vector2f getSize() const;
};
} // namespace GUI