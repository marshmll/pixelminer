/**
 * @file PlayerGUI.hxx
 * @brief Declares the PlayerGUI class to manage player specific GUI.
 */

#pragma once

#include "Engine/ResourcePack.hxx"
#include "Entities/Playable/Player.hxx"
#include "Player/Hotbar.hxx"

/**
 * @class PlayerGUI
 * @brief A class that manages the graphical user interface (GUI) elements for the player.
 *
 * The `PlayerGUI` class is responsible for initializing, updating, and rendering the player-related
 * GUI elements, such as the hotbar. It interacts with the `Player` class to reflect the player's
 * current attributes and state in the GUI.
 */
class PlayerGUI
{
  private:
    /**
     * @brief A reference to the player object.
     */
    Player &player;

    /**
     * @brief A unique pointer to the hotbar GUI element.
     */
    std::unique_ptr<Hotbar> hotbar;

    sf::RectangleShape tileHoverIndicator;

    /**
     * @brief A shared pointer to the resource pack containing textures for the GUI.
     */
    std::shared_ptr<ResourcePack> &resourcePack;

    /**
     * @brief The video mode used for rendering the GUI.
     */
    sf::VideoMode &vm;

    /**
     * @brief The scale factor used for scaling GUI elements.
     */
    unsigned int scale;

    /**
     * @brief Initializes the graphical user interface (GUI) elements for the player.
     *
     * This method initializes the hotbar as part of the player GUI setup.
     */
    void initGUI();

  public:
    /**
     * @brief Constructs a PlayerGUI instance.
     *
     * @param player A reference to the player object to link to the GUI.
     * @param resource_pack A shared pointer to the resource pack containing GUI textures.
     * @param vm The video mode of the window for determining screen size.
     * @param scale The scale factor for rendering GUI elements.
     */
    PlayerGUI(Player &player, std::shared_ptr<ResourcePack> &resource_pack, sf::VideoMode &vm,
              const unsigned int &scale);

    /**
     * @brief Destroys the PlayerGUI instance.
     *
     * The destructor cleans up any resources related to the GUI.
     */
    ~PlayerGUI();

    /**
     * @brief Updates the player GUI elements.
     *
     * This method updates the hotbar and other GUI elements based on the time elapsed.
     *
     * @param dt The delta time since the last update.
     */
    void update(const float &dt, const sf::Vector2i &mouse_pos_grid, std::optional<sf::Event::MouseWheelScrolled> &mouse_data);

    /**
     * @brief Renders the player GUI elements to the screen.
     *
     * This method renders the hotbar and other GUI components to the specified render target.
     *
     * @param target The render target to draw the player GUI elements on.
     */
    void render(sf::RenderTarget &target);

    void updateTileHoverIndicator(const sf::Vector2i &mouse_pos_grid);

    void renderTileHoverIndicator(sf::RenderTarget &target);

    const bool hasTileHovered() const;

    const sf::Vector2i getHoveredTileGridPosition();

    sf::RectangleShape &getTileHoverIndicator();
};
