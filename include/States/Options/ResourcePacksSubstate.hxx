#pragma once

#include "Engine/ResourcePack.hxx"
#include "States/Substate.hxx"
#include "GUI/GUI.hxx"
#include "Tools/FileExplorer.hxx"

/**
 * @class ResourcePackSelector
 * @brief A class representing a selectable resource pack in the resource pack selection menu.
 *  * This class is responsible for displaying and handling interactions with individual resource pack selectors,
 * including rendering and selection logic.
 */
class ResourcePackSelector
{
  public:
    EngineData &data;                      ///< Engine data used for various purposes, like window size and textures.
    ResourcePack &resourcePack;            ///< Resource pack reference.
    sf::RectangleShape container;          ///< Container shape for the resource pack selector.
    sf::RectangleShape icon;               ///< Icon shape representing the resource pack.
    std::shared_ptr<sf::Text> name;        ///< Name of the resource pack displayed.
    std::shared_ptr<sf::Text> description; ///< Description of the resource pack displayed.
    std::shared_ptr<gui::ImageButton> btn; ///< A button to allow enabling/disabling the resource pack.
    bool pressed;                          ///< Flag to indicate if the selector is pressed.

    /**
     * @brief Constructs a ResourcePackSelector object.
     * @param data Engine data reference for accessing resources like textures and fonts.
     * @param position The position for the resource pack selector in the menu.
     * @param resource_pack The resource pack reference.
     */
    ResourcePackSelector(EngineData &data, const sf::Vector2f &position, const sf::Vector2f &size,
                         ResourcePack &resource_pack)
        : data(data), resourcePack(resource_pack), pressed(false)
    {
        container.setSize(size);
        container.setPosition(sf::Vector2f(static_cast<int>(position.x), static_cast<int>(position.y)));
        container.setFillColor(sf::Color::Transparent);
        container.setOutlineColor(sf::Color::White);

        icon.setSize(sf::Vector2f(sf::Vector2f(container.getSize().y, container.getSize().y)));
        icon.setPosition(container.getPosition());
        icon.setTexture(&resource_pack.icon);

        name = std::make_shared<sf::Text>(data.activeResourcePack->getFont("Bold"),
                                          sf::String::fromUtf8(resource_pack.name.begin(), resource_pack.name.end()),
                                          gui::charSize(*data.vm, 120));

        name->setPosition(
            sf::Vector2f(static_cast<int>(icon.getPosition().x + icon.getSize().x + gui::percent(data.vm->size.x, 1.f)),
                         static_cast<int>(icon.getPosition().y + gui::percent(data.vm->size.y, 1.f))));

        name->setFillColor(sf::Color::White);

        description = std::make_shared<sf::Text>(
            data.activeResourcePack->getFont("Regular"),
            sf::String::fromUtf8(resource_pack.description.begin(), resource_pack.description.end()),
            gui::charSize(*data.vm, 120));

        description->setPosition(sf::Vector2f(static_cast<int>(name->getPosition().x),
                                              static_cast<int>(name->getPosition().y + name->getGlobalBounds().size.y +
                                                               gui::percent(data.vm->size.y, 1.5f))));

        description->setFillColor(sf::Color(200, 200, 200, 255));

        btn = std::make_shared<gui::ImageButton>(icon.getPosition(), icon.getSize(), sf::Color::Transparent,
                                                 data.activeResourcePack->getTexture("ButtonArrowRight"),
                                                 sf::Vector2f(icon.getSize() * .8f));
    }

    /**
     * @brief Updates the state of the resource pack selector based on mouse interaction.
     * @param dt Delta time for frame-based updates.
     * @param mouse_pos Current mouse position.
     */
    inline void update(const float &dt, const sf::Vector2f &mouse_pos)
    {
        pressed = false;

        btn->update(mouse_pos);

        if (container.getGlobalBounds().contains(mouse_pos))
        {
            btn->setState(gui::ButtonState::Hover);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                pressed = true;
        }
    }

    /**
     * @brief Renders the resource pack selector to the target.
     * @param target The render target where the resource pack selector will be drawn.
     */
    inline void render(sf::RenderTarget &target)
    {
        target.draw(container);
        target.draw(icon);

        if (btn->getState() != gui::ButtonState::Idle)
            btn->render(target);

        target.draw(*name);
        target.draw(*description);
    }

    /**
     * @brief Gets the position of the resource pack selector container.
     * @return Position of the resource pack selector container.
     */
    inline const sf::Vector2f getPosition() const
    {
        return container.getPosition();
    }

    /**
     * @brief Gets the size of the resource pack selector container.
     * @return Size of the resource pack selector container.
     */
    inline const sf::Vector2f getSize() const
    {
        return container.getSize();
    }

    inline const bool isPressed()
    {
        return pressed;
    }

    inline void setPosition(const sf::Vector2f &position)
    {
        sf::Vector2f off = position - container.getPosition();

        container.move(off);
        icon.move(off);
        name->move(off);
        description->move(off);
        btn->move(off);
    }
};

class ResourcePacksSubstate : public Substate
{
  private:
    sf::RectangleShape background;

    sf::RectangleShape buttonContainer;
    std::unordered_map<std::string, std::unique_ptr<gui::Button>> buttons;

    std::unique_ptr<gui::ScrollableContainer> availableContainer;
    std::unique_ptr<gui::ScrollableContainer> enabledContainer;

    std::unique_ptr<sf::Text> availableText;
    std::unique_ptr<sf::Text> enabledText;

    std::vector<std::shared_ptr<ResourcePackSelector>> selectors;
    std::vector<std::shared_ptr<ResourcePackSelector>> availableResourcePackSelectors;
    std::vector<std::shared_ptr<ResourcePackSelector>> enabledResourcePackSelectors;

    void initGUI();

    void initButtons();

    void initResourcePackSelectors();

    const std::string getEnabledPacksPriorityString();

  public:
    ResourcePacksSubstate(EngineData &data);

    ~ResourcePacksSubstate();

    void update(const float &dt);

    void updateAvailableSelectors(const float &dt);

    void updateEnabledSelectors(const float &dt);

    void render(sf::RenderTarget &target);
};
