#include "stdafx.hxx"
#include "GUI/ToggleButton.hxx"

using namespace gui;

ToggleButton::ToggleButton(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color,
                           const std::string toggle_msg, const sf::Font &font, const unsigned int char_size,
                           const sf::Color text_color, const bool &on, const uint32_t toggle_time_max,
                           const float outline_thickness, const sf::Color outline_color)
    : TextButton(position, size, fill_color, toggle_msg + ": " + (on ? _("ON") : _("OFF")), font, char_size, text_color,
                 outline_thickness, outline_color),
      toggleMsg(toggle_msg), toggleTimeMax(toggle_time_max), on(on)
{
    toggleClock.restart();
}

void ToggleButton::update(const sf::Vector2f &mouse_pos)
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

            if (toggleClock.getElapsedTime().asMilliseconds() >= toggleTimeMax)
            {
                toggleOn();
                toggleClock.restart();
            }
        }
    }
}

ToggleButton::~ToggleButton() = default;

const bool &ToggleButton::isOn() const
{
    return on;
}

void ToggleButton::setOn(const bool &on)
{
    this->on = on;
    setString(toggleMsg + ": " + (on ? _("ON") : _("OFF")));
}

void ToggleButton::toggleOn()
{
    setOn(!on);
}
