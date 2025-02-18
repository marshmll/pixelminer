#pragma once

#include "GUI/Button.hxx"
#include "GUI/Utils.hxx"

namespace gui
{
    class ImageButton : public Button
    {
      private:
        sf::RectangleShape imageRect;

      public:
        ImageButton(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &fill_color,
                    sf::Texture &image, const sf::Vector2f &image_size, const float &outline_thickness = 0.f,
                    const sf::Color &outline_color = sf::Color::Transparent);

        ~ImageButton();

        void render(sf::RenderTarget &target) override;
    };
} // namespace gui