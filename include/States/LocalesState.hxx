/**
 * @file LocalesState.hxx
 * @brief Declares the LocalesState class for changing the game locale/language.
 */

#pragma once

#include "GUI/GUI.hxx"
#include "States/State.hxx"
#include "States/MessageState.hxx"

/**
 * @class LangOption
 * @brief Internal class that represents a option of a locale available for selection.
 */
class LangOption
{
  private:
    sf::RectangleShape box; ///< A box representing the option container.
    sf::Text text;          ///< The inner text of the option.
    std::string value;      ///< The locale value in the format `xx_XX.UTF-8`.
    bool selected;          ///< If the option is selected.

  public:
    /**
     * @brief Constructor for the LangOption class.
     * @param name The name of the option. This is the text shown to the user.
     * @param value The locale value of the option in the format `xx_XX.UTF-8`.
     * @param font A font reference.
     * @param char_size The character size.
     * @param position The container's position.
     * @param size The container's size.
     * @param selected If the option should be already selected (default false).
     */
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

    /**
     * @brief Updates the lang option.
     * @param mouse_pos The mouse position relative to the view.
     */
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

    /**
     * @brief Renders the option into a target.
     * @param target The target to render the option to.
     */
    inline void render(sf::RenderTarget &target)
    {
        target.draw(box);
        target.draw(text);
    }

    /**
     * @brief Gets the name of the option as a `sf::String`.
     * @return The name of the option as a `sf::String`.
     */
    inline const sf::String &getName() const
    {
        return text.getString();
    }

    /**
     * @brief Gets the value of the option as a `std::string`.
     * @return The value of the option as a `std::string`.
     */
    inline const std::string &getValue() const
    {
        return value;
    }

    /**
     * @brief Gets the position of the container.
     * @return The position of the container.
     */
    inline const sf::Vector2f getPosition()
    {
        return box.getPosition();
    }

    /**
     * @brief Gets the size of the container.
     * @return The size of the container.
     */
    inline const sf::Vector2f getSize()
    {
        return box.getSize();
    }

    /**
     * @brief Checks if the option is selected.
     * @return The selected flag.
     */
    inline const bool isSelected() const
    {
        return selected;
    }

    /**
     * @brief Sets a new value for the selected flag.
     * @param selected The new value for the selected flag.
     */
    inline void setSelected(const bool &selected)
    {
        this->selected = selected;
    }
};

/**
 * @class LocalesState
 * @brief This state manages the user's locale/language selection among the available translations declared in the
 * `locales.json` file and that are present as Machine Objects (`.mo` files) in the `Locales` folder.
 */
class LocalesState : public State
{
  private:
    sf::RectangleShape background;            ///< Background rectangle of the menu.
    sf::RectangleShape header;                ///< Header section rectangle of the menu.
    sf::RectangleShape footer;                ///< Footer section rectangle of the menu.
    sf::RectangleShape buttonContainer;       ///< A container used to position and size button.
    std::unique_ptr<sf::Text> disclaimerText; ///< A text warning for the user.

    std::unique_ptr<gui::ScrollableContainer> scrollContainer; ///< A scrollable container to deal with option scrolling.

    std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons; ///< A `std::unordered_map` of buttons.

    std::vector<LangOption> options; ///< A vector of LangOptions containing all options.
    LangOption *selectedOption;      ///< A raw pointer to the selected option.

    JObject localesObj; ///< The parsed `locales.json` file as a JSON object.

    /**
     * @brief Reads the `locales.json` file and parses it into a JObject.
     *
     * If there is an error while parsing, the state is replaced by a MessageState.
     */
    void readLocales();

    /**
     * @brief Initializes the GUI for the LocalesState.
     */
    void initGUI();

    /**
     * @brief Saves the current selected locale to the `locales.json` file.
     *
     * If there is an error while saving, the state is replaced by a MessageState.
     */
    const bool saveNewLocale();

  public:
    /**
     * @brief Constructor for the LocalesState class.
     * @param data The engine data reference.
     */
    LocalesState(EngineData &data);

    /**
     * @brief Destructor for the LocalesState class.
     */
    ~LocalesState();

    /**
     * @brief Updates the LocaleState class.
     * @param dt The delta time since last frame render.
     */
    void update(const float &dt);

    /**
     * @brief Renders the LocaleState class into a target.
     * @param target a target to render the state to.
     */
    void render(sf::RenderTarget &target);
};
