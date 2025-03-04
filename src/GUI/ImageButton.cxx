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
    imageRect.setPosition(sf::Vector2f(body.getPosition().x + 1.f + body.getSize().x / 2.f - image_size.x / 2.f,
                                       body.getPosition().y + 1.f + body.getSize().y / 2.f - image_size.y / 2.f));
}

ImageButton::~ImageButton() = default;

void ImageButton::render(sf::RenderTarget &target)
{
    target.draw(darkShadow);
    target.draw(body);
    target.draw(brightShadow);
    target.draw(imageRect);
}

void ImageButton::setImage(sf::Texture &image)
{
    imageRect.setTexture(&image);
}

void ImageButton::move(const sf::Vector2f &offset)
{
    darkShadow.move(offset);
    body.move(offset);
    brightShadow.move(offset);
    imageRect.move(offset);
}
