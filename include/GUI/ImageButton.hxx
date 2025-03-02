/**
 * @file ImageButton.hxx
 * @brief Declares the ImageButton class which is a type of button containing a sprite as its content.
 */

#pragma once

#include "GUI/Button.hxx"
#include "GUI/Utils.hxx"

namespace gui
{
    /**
     * @class ImageButton
     * @brief The image button is a type of a interactive button which has a sprite inside of it.
     *
     * It can be used to any generic sprite and the button's body does not strictly need to be of
     * the size of the sprite. Managing sizes is the user's responsability.
     */
    class ImageButton : public Button
    {
      private:
        sf::RectangleShape imageRect; ///< The rectangle that contains the image.

      public:

        /**
         * @brief Constructor for the ImageButton class.
         * @param position The position of the button.
         * @param size The size of the button.
         * @param fill_color The color of the button's body.
         * @param image The image to use inside the button.
         * @param image_size The image size of the button.
         * @param outline_thickness The thickness of the outline (default is 0.f).
         * @param outline_color The color of the outline (default is transparent).
         *
         * @note Using a not-scaled size to the `image_size` param can cause distortions to the image.
         */
        ImageButton(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &fill_color,
                    sf::Texture &image, const sf::Vector2f &image_size, const float &outline_thickness = 0.f,
                    const sf::Color &outline_color = sf::Color::Transparent);

        ~ImageButton();

        void render(sf::RenderTarget &target) override;
    };
} // namespace gui
