#pragma once

#include "GUI/GUI.hxx"
#include "States/State.hxx"
#include "States/MessageState.hxx"

class LangOption
{
  private:
    sf::RectangleShape box;
    sf::Text text;
    std::string value;
    bool selected;

  public:
    LangOption(const sf::String &name, const std::string &value, sf::Font &font, const unsigned int &char_size,
               const sf::Vector2f &position, const sf::Vector2f &size, const bool &selected = false)
        : text(font, name, char_size), value(value), selected(selected)
    {
        box.setSize(size);
        box.setPosition(position);
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineColor(sf::Color::White);

        text.setPosition(
            sf::Vector2f(std::floor(box.getPosition().x + box.getSize().x / 2.f - text.getGlobalBounds().size.x / 2.f),
                         std::floor(box.getPosition().y + box.getSize().y / 2.f - text.getGlobalBounds().size.y / 2.f -
                                    char_size / 4.f)));
    }

    inline void update(const sf::Vector2f &mouse_pos)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (box.getGlobalBounds().contains(mouse_pos))
                selected = true;
        }

        if (selected)
            box.setOutlineThickness(1.f);
        else
            box.setOutlineThickness(0.f);
    }

    inline void render(sf::RenderTarget &target)
    {
        target.draw(box);
        target.draw(text);
    }

    inline const std::string getName() const
    {
        return text.getString();
    }

    inline const std::string &getValue() const
    {
        return value;
    }

    inline const sf::Vector2f getPosition()
    {
        return box.getPosition();
    }

    inline const sf::Vector2f getSize()
    {
        return box.getSize();
    }

    inline const bool isSelected() const
    {
        return selected;
    }

    inline void setSelected(const bool &selected)
    {
        this->selected = selected;
    }
};

class LocalesState : public State
{
  private:
    sf::RectangleShape background; ///< Background rectangle of the menu.
    sf::RectangleShape header;     ///< Header section rectangle of the menu.
    sf::RectangleShape footer;     ///< Footer section rectangle of the menu.
    sf::RectangleShape buttonContainer;
    std::unique_ptr<gui::ScrollableContainer> scrollContainer;

    std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons;

    std::vector<LangOption> options;
    LangOption *selectedOption;

    JObject localesObj;

    void readLocales();

    void initGUI();

    const bool saveNewLocale();

  public:
    LocalesState(EngineData &data);

    ~LocalesState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);
};
