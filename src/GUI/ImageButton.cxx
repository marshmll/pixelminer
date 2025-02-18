#include "stdafx.hxx"
#include "GUI/ImageButton.hxx"

using namespace gui;

ImageButton::ImageButton(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &fill_color,
                         sf::Texture &image, const sf::Vector2f &image_size, const float &outline_thickness,
                         const sf::Color &outline_color)
    : Button(position, size, fill_color, outline_thickness, outline_color)
{
    imageRect.setSize(image_size);
    imageRect.setTexture(&image);
    imageRect.setPosition(sf::Vector2f(getPosition().x + getSize().x / 2.f - imageRect.getSize().x / 2.f,
                                       getPosition().y + getSize().y / 2.f - imageRect.getSize().y / 2.f));
}

ImageButton::~ImageButton() = default;

void ImageButton::render(sf::RenderTarget &target)
{
    target.draw(darkShadow);
    target.draw(body);
    target.draw(brightShadow);
    target.draw(imageRect);
}