#pragma once

#include "GUI/GUI.hxx"
#include "GUI/TextButton.hxx"

namespace gui
{
    class ToggleButton : public TextButton
    {
      private:
        std::string toggleMsg;
        sf::Clock toggleClock;
        uint32_t toggleTimeMax;
        bool on;

      public:
        ToggleButton(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color,
                     const std::string toggle_msg, const sf::Font &font, const unsigned int char_size,
                     const sf::Color text_color = sf::Color::White, const bool &on = false, const uint32_t toggle_time_max = 200,
                     const float outline_thickness = 0.f, const sf::Color outline_color = sf::Color::Transparent);

        ~ToggleButton();

        void update(const sf::Vector2f &mouse_pos);

        const bool &isOn() const;

        void setOn(const bool &on);

        void toggleOn();
    };
} // namespace gui
