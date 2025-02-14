/**
 * @file WorldSelectionMenuState.hxx
 * @brief Declares the WorldSelectionMenuState class for visualizing and playing in local worlds.
 */

#pragma once

#include "GUI/GUI.hxx"
#include "States/Singleplayer/GameState.hxx"
#include "States/State.hxx"
#include "Tools/Date.hxx"
#include "Tools/JSON.hxx"

/**
 * @struct WorldMetadata
 * @brief A structure that holds metadata for a world, including the name, folder, creation date, and other details.
 */
struct WorldMetadata
{
    std::string worldName;           ///< Name of the world.
    std::string folderName;          ///< Folder name of the world.
    unsigned long long creationDate; ///< Creation date of the world (in Unix timestamp format).
    unsigned long long lastPlayed;   ///< Last played date of the world (in Unix timestamp format).
    std::string gameVersion;         ///< Version of the game associated with the world.
    std::string difficulty;          ///< Difficulty setting of the world.
};

/**
 * @class WorldSelectionMenuState
 * @brief The state for displaying and managing the world selection menu.
 *
 * This class allows the player to select, create, edit, or delete worlds, and provides options to manage the
 * selected world. It contains functionality to display, interact with, and update world selectors.
 */
class WorldSelectionMenuState : public State
{
  private:
    /**
     * @class WorldSelector
     * @brief A class representing a selectable world in the world selection menu.
     *
     * This class is responsible for displaying and handling interactions with individual world selectors,
     * including rendering and selection logic.
     */
    class WorldSelector
    {
      public:
        EngineData &data;               ///< Engine data used for various purposes, like window size and textures.
        sf::Texture iconTexture;        ///< Texture of the world icon.
        WorldMetadata metadata;         ///< Metadata of the world associated with this selector.
        sf::RectangleShape container;   ///< Container shape for the world selector.
        sf::RectangleShape icon;        ///< Icon shape representing the world.
        std::unique_ptr<sf::Text> name; ///< Name of the world displayed.
        std::unique_ptr<sf::Text> description; ///< Description of the world displayed.

        bool selected; ///< Flag indicating if the world is selected.

        /**
         * @brief Constructs a WorldSelector object.
         * @param data Engine data reference for accessing resources like textures and fonts.
         * @param metadata Metadata associated with the world.
         * @param y_position The vertical position for the world selector in the menu.
         */
        WorldSelector(EngineData &data, WorldMetadata metadata, const float y_position)
            : data(data), metadata(metadata), selected(false)
        {
            container.setSize(sf::Vector2f(gui::percent(data.vm->size.x, 60.f), gui::percent(data.vm->size.y, 18.f)));
            container.setPosition(sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - container.getSize().x / 2.f),
                                               static_cast<int>(y_position)));
            container.setFillColor(sf::Color(0, 0, 0, 80));
            container.setOutlineColor(sf::Color::White);

            if (!iconTexture.loadFromFile(MAPS_FOLDER + metadata.folderName + "/icon.png"))
                std::cerr << "Error while loading image: " << MAPS_FOLDER + metadata.folderName + "/icon.png" << "\n";

            icon.setSize(sf::Vector2f(sf::Vector2f(container.getSize().y, container.getSize().y)));
            icon.setPosition(container.getPosition());
            icon.setTexture(&iconTexture);

            name = std::make_unique<sf::Text>(data.activeResourcePack->fonts.at("Bold"), metadata.worldName,
                                              gui::charSize(*data.vm, 100));

            name->setPosition(sf::Vector2f(
                static_cast<int>(icon.getPosition().x + icon.getSize().x + gui::percent(data.vm->size.x, 1.f)),
                static_cast<int>(icon.getPosition().y + gui::percent(data.vm->size.y, 1.f))));

            name->setFillColor(sf::Color::White);

            description = std::make_unique<sf::Text>(
                data.activeResourcePack->fonts.at("Regular"),
                metadata.folderName + " (" + Date::toLocaleString(metadata.creationDate) +
                    "\nLast Played: " + Date::toLocaleString(metadata.lastPlayed) +
                    ")\nGame Version: " + metadata.gameVersion + ", Difficulty: " + metadata.difficulty,
                gui::charSize(*data.vm, 100));

            description->setPosition(
                sf::Vector2f(static_cast<int>(name->getPosition().x),
                             static_cast<int>(name->getPosition().y + gui::percent(data.vm->size.y, 5.f))));

            description->setFillColor(sf::Color(200, 200, 200, 255));
        }

        /**
         * @brief Updates the state of the world selector based on mouse interaction.
         * @param dt Delta time for frame-based updates.
         * @param mouse_pos Current mouse position.
         */
        inline void update(const float &dt, const sf::Vector2f &mouse_pos)
        {
            if (!selected)
                container.setOutlineThickness(0.f);

            if (container.getGlobalBounds().contains(mouse_pos))
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    selected = true;
                    container.setOutlineThickness(1.f);
                }
            }
        }

        /**
         * @brief Renders the world selector to the target.
         * @param target The render target where the world selector will be drawn.
         */
        inline void render(sf::RenderTarget &target)
        {
            target.draw(container);
            target.draw(icon);
            target.draw(*name);
            target.draw(*description);
        }

        /**
         * @brief Gets the position of the world selector container.
         * @return Position of the world selector container.
         */
        inline const sf::Vector2f getPosition() const
        {
            return container.getPosition();
        }

        /**
         * @brief Gets the size of the world selector container.
         * @return Size of the world selector container.
         */
        inline const sf::Vector2f getSize() const
        {
            return container.getSize();
        }

        /**
         * @brief Checks if the world selector is selected.
         * @return True if selected, false otherwise.
         */
        inline const bool isSelected() const
        {
            return selected;
        }

        /**
         * @brief Sets the selection state of the world selector.
         * @param selected The new selection state.
         */
        inline void setSelected(const bool selected)
        {
            this->selected = selected;

            if (!selected)
                container.setOutlineThickness(0.f);
            else
                container.setOutlineThickness(1.f);
        }
    };

    sf::RectangleShape background; ///< Background rectangle of the menu.
    sf::RectangleShape header;     ///< Header section rectangle of the menu.
    sf::RectangleShape footer;     ///< Footer section rectangle of the menu.

    sf::RectangleShape buttonContainer; ///< Container for buttons in the footer.

    std::map<std::string, std::unique_ptr<gui::TextButton>> buttons; ///< Map of buttons in the menu.

    std::vector<std::shared_ptr<WorldSelector>> worldSelectors;   ///< List of world selectors.
    std::unique_ptr<gui::ScrollableContainer> worldSelectorsList; ///< Scrollable container for the world selectors.

    std::optional<std::shared_ptr<WorldSelector>> selectedWorld; ///< Currently selected world, if any.

    /**
     * @brief Initializes the GUI components for the world selection menu.
     */
    void initGUI();

    /**
     * @brief Initializes the world selectors based on available world metadata.
     */
    void initWorldSelectors();

  public:
    /**
     * @brief Constructs the WorldSelectionMenuState object.
     * @param data Engine data reference for accessing resources and managing the state.
     */
    WorldSelectionMenuState(EngineData &data);

    /**
     * @brief Destroys the WorldSelectionMenuState object.
     */
    ~WorldSelectionMenuState();

    /**
     * @brief Updates the world selection menu state.
     * @param dt Delta time for frame-based updates.
     */
    void update(const float &dt);

    /**
     * @brief Renders the world selection menu to the target.
     * @param target The render target where the world selection menu will be drawn.
     */
    void render(sf::RenderTarget &target);

    /**
     * @brief Updates the GUI components of the world selection menu.
     * @param dt Delta time for frame-based updates.
     */
    void updateGUI(const float &dt);

    /**
     * @brief Renders the GUI components of the world selection menu.
     * @param target The render target where the GUI will be drawn.
     */
    void renderGUI(sf::RenderTarget &target);
};
