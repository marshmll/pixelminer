#pragma once

#include "GUI/Button.hxx"
#include "GUI/Utils.hxx"

namespace gui
{
    class SliderInput
    {
      private:
        sf::RectangleShape body;
        std::unique_ptr<gui::Button> slider;
        std::unique_ptr<sf::Text> text;
        std::unique_ptr<sf::Text> textShadow;

        int selectedOption;
        unsigned int maxOption;

      public:
        SliderInput(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &body_color,
                    const sf::Color &outline_color, const float &outline_thickness, const float &slider_thickness,
                    const sf::Color &slider_color, const sf::Color slider_outline_color,
                    const float &slider_outline_thickness, sf::Font &font, const std::string &str,
                    const unsigned int &char_size, const unsigned int &max_option,
                    const unsigned int selected_option = 0);

        ~SliderInput();

        void update(const sf::Vector2f &mouse_pos);

        void render(sf::RenderTarget &target);

        const int &getSelectedOption() const;

        const unsigned int &getMaxOption() const;

        void setString(const std::string &str);
    };
} // namespace gui
