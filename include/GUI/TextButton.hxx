#pragma once

#include "GUI/Button.hxx"

namespace GUI
{
class TextButton : public Button
{
  private:
    std::unique_ptr<sf::Text> text;
    sf::Color textColor;

  public:
    TextButton(const sf::Vector2f position, const sf::Vector2f size, const sf::Color fill_color, const std::string str,
               const sf::Font &font, const unsigned int char_size, const sf::Color text_color = sf::Color::White,
               const float outline_thickness = 0.f, const sf::Color outline_color = sf::Color::Transparent);

    ~TextButton();

    void update(const sf::Vector2f &mouse_pos);

    void render(sf::RenderTarget &target);
};
} // namespace GUI
