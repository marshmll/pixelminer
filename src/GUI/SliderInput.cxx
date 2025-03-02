#include "stdafx.hxx"
#include "GUI/SliderInput.hxx"

using namespace gui;

SliderInput::SliderInput(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &body_color,
                         const sf::Color &outline_color, const float &outline_thickness, const float &slider_thickness,
                         const sf::Color &slider_color, const sf::Color slider_outline_color,
                         const float &slider_outline_thickness, sf::Font &font, const std::string &str,
                         const unsigned int &char_size, const unsigned int &max_option,
                         const unsigned int selected_option)
    : maxOption(max_option), selectedOption(selected_option)
{
    body.setSize(size);
    body.setPosition(position);
    body.setFillColor(body_color);
    body.setOutlineColor(outline_color);
    body.setOutlineThickness(outline_thickness);

    slider = std::make_unique<gui::Button>(
        sf::Vector2f(body.getPosition().x +
                         (body.getSize().x - slider_thickness) * (selectedOption / static_cast<float>(maxOption - 1)),
                     body.getPosition().y),
        sf::Vector2f(slider_thickness, size.y - slider_outline_thickness), slider_color, slider_outline_thickness,
        slider_outline_color);

    text = std::make_unique<sf::Text>(font, sf::String::fromUtf8(str.begin(), str.end()), char_size);
    text->setPosition(sf::Vector2f(body.getPosition().x + body.getSize().x / 2.f - text->getGlobalBounds().size.x / 2.f,
                                   body.getPosition().y + body.getSize().y / 2.f -
                                       text->getGlobalBounds().size.y / 2.f - char_size / 6.f));

    text->setFillColor(sf::Color::White);

    textShadow = std::make_unique<sf::Text>(font, sf::String::fromUtf8(str.begin(), str.end()), char_size);
    textShadow->setPosition(sf::Vector2f(
        static_cast<int>((body.getPosition().x + body.getSize().x / 2.f - text->getGlobalBounds().size.x / 2.f) - 2.f),
        static_cast<int>(
            (body.getPosition().y + body.getSize().y / 2.f - text->getGlobalBounds().size.y / 2.f - char_size / 6.f) -
            2.f)));

    textShadow->setFillColor(sf::Color(0, 0, 0, 170));
}

SliderInput::~SliderInput() = default;

void SliderInput::update(const sf::Vector2f &mouse_pos)
{
    slider->setState(gui::ButtonState::Idle);

    if (body.getGlobalBounds().contains(mouse_pos))
    {
        slider->setState(gui::ButtonState::Hover);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            slider->setState(gui::ButtonState::Active);

            float percent = (mouse_pos.x - body.getPosition().x) / body.getSize().x;
            selectedOption = static_cast<unsigned int>(std::roundf((maxOption - 1) * percent));

            if (selectedOption < 0)
                selectedOption = 0;
            else if (selectedOption >= maxOption)
                selectedOption = maxOption - 1;

            // setString(std::to_string(selectedOption));

            slider->setPosition(
                sf::Vector2f(body.getPosition().x + (body.getSize().x - slider->getSize().x) *
                                                        (selectedOption / static_cast<float>(maxOption - 1)),
                             body.getPosition().y));
        }
    }
}

void SliderInput::render(sf::RenderTarget &target)
{
    target.draw(body);
    slider->render(target);
    target.draw(*textShadow);
    target.draw(*text);
}

const int &SliderInput::getSelectedOption() const
{
    return selectedOption;
}

const unsigned int &SliderInput::getMaxOption() const
{
    return maxOption;
}

void SliderInput::setString(const std::string &str)
{
    text->setString(sf::String::fromUtf8(str.begin(), str.end()));
    textShadow->setString(sf::String::fromUtf8(str.begin(), str.end()));

    text->setPosition(sf::Vector2f(body.getPosition().x + body.getSize().x / 2.f - text->getGlobalBounds().size.x / 2.f,
                                   body.getPosition().y + body.getSize().y / 2.f -
                                       text->getGlobalBounds().size.y / 2.f - text->getCharacterSize() / 6.f));

    textShadow->setPosition(sf::Vector2f(
        static_cast<int>((body.getPosition().x + body.getSize().x / 2.f - text->getGlobalBounds().size.x / 2.f) - 2.f),
        static_cast<int>((body.getPosition().y + body.getSize().y / 2.f - text->getGlobalBounds().size.y / 2.f -
                          text->getCharacterSize() / 6.f) -
                         2.f)));
}
