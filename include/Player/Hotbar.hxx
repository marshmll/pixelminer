/**
 * @file Hotbar.hxx
 * @brief Declares the HotBar class to manage player hotbar GUI.
 */

#pragma once

#include "Engine/ResourcePack.hxx"
#include "Entities/Functionalities/AttributeFunctionality.hxx"
#include "GUI/GUI.hxx"

/**
 * @brief Represents how many slots the hotbar has.
 */
constexpr uint8_t MAX_HOTBAR_SLOTS = 9;

/**
 * @class Hotbar
 * @brief A class that represents and manages the player's hotbar, including health and hunger bars.
 *
 * The `Hotbar` class manages the graphical elements and logic for the player's hotbar, including health
 * and hunger bars. It interacts with the player's attributes to display the current status of health and
 * hunger using sprites.
 */
class Hotbar
{
  private:
    /**
     * @brief Reference to the player's attribute functionality (health, hunger, etc.).
     */
    AttributeFunctionality &playerAttributes;

    /**
     * @brief A shared pointer to the resource pack containing the textures used for the hotbar.
     */
    std::shared_ptr<ResourcePack> &resourcePack;

    /**
     * @brief The video mode used to determine screen size for rendering.
     */
    sf::VideoMode &vm;

    /**
     * @brief Maximum number of columns for health and hunger bars.
     */
    uint8_t maxCols;

    /**
     * @brief The number of rows for the health bar.
     */
    uint8_t healthBarRows;

    /**
     * @brief The number of rows for the hunger bar.
     */
    uint8_t hungerBarRows;

    /**
     * @brief The sprite of the hotbar slots.
     */
    std::unique_ptr<sf::Sprite> hotbar;

    /**
     * @brief The sprite of the hotbar slot selector.
     */
    std::unique_ptr<sf::Sprite> hotbarSelector;

    /**
     * @brief Which hotbar slot is selected from 0 to `MAX_HOTBAR_SLOTS` - 1;
     */
    int8_t hotbarSlot;

    /**
     * @brief Scale factor for the sprites in the hotbar.
     */
    unsigned int scale;

    /**
     * @brief A vector of sprites representing the health bar.
     */
    std::vector<sf::Sprite> healthBar;

    /**
     * @brief A vector of sprites representing the hunger bar.
     */
    std::vector<sf::Sprite> hungerBar;

    /**
     * @brief Initializes the graphical user interface (GUI) elements for the hotbar.
     *
     * This method sets up the visual elements of the hotbar, including positioning and scaling
     * of health and hunger bar sprites.
     */
    void initGUI();

    /**
     * @brief Updates the hotbar slot selector.
     */
    void updateHotbarSlotSelector();

  public:
    /**
     * @brief Constructs a Hotbar instance.
     *
     * @param player_attributes A reference to the player's attribute functionality.
     * @param resorce_pack A shared pointer to the resource pack containing textures.
     * @param vm The video mode of the window.
     * @param scale The scale factor for the sprites.
     */
    Hotbar(AttributeFunctionality &player_attributes, std::shared_ptr<ResourcePack> &resorce_pack, sf::VideoMode &vm,
           const unsigned int &scale);

    /**
     * @brief Destroys the Hotbar instance.
     *
     * The destructor cleans up the hotbar resources.
     */
    ~Hotbar();

    /**
     * @brief Updates the hotbar elements.
     *
     * This method updates the health and hunger bars based on the player's current attributes.
     *
     * @param dt The delta time since the last update.
     */
    void update(const float &dt, std::optional<sf::Event::MouseWheelScrolled> &mouse_data);

    /**
     * @brief Renders the hotbar on the target.
     *
     * This method renders both the health and hunger bars to the given render target.
     *
     * @param target The render target to draw the hotbar elements.
     */
    void render(sf::RenderTarget &target);

    /**
     * @brief Updates the health bar sprites based on the player's current health.
     */
    void updateHealthBar();

    /**
     * @brief Updates the hunger bar sprites based on the player's current hunger.
     */
    void updateHungerBar();

    /**
     * @brief Updates the hotbar.
     */
    void updateHotbar(std::optional<sf::Event::MouseWheelScrolled> mouse_data);

    /**
     * @brief Renders the health bar on the target.
     *
     * This method draws all health bar sprites to the given render target.
     *
     * @param target The render target to draw the health bar.
     */
    void renderHealthBar(sf::RenderTarget &target);

    /**
     * @brief Renders the hunger bar on the target.
     *
     * This method draws all hunger bar sprites to the given render target.
     *
     * @param target The render target to draw the hunger bar.
     */
    void renderHungerBar(sf::RenderTarget &target);

    /**
     * @brief Renders the hotbar.
     */
    void renderHotbar(sf::RenderTarget &target);
};
