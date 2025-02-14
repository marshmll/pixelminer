#pragma once

#include "GUI/GUI.hxx"
#include "States/Multiplayer/AddServerState.hxx"
#include "States/Multiplayer/DirectConnectState.hxx"
#include "States/State.hxx"

struct ServerMetadata
{
    std::string serverName;        ///< Name of the server.
    std::string serverAddress;     ///< The address of the server.
    std::string serverDescription; ///< A short description of the server,
    std::string gameVersion;       ///< Version of the game associated with the server.
    std::string status;            ///< Version of the game associated with the server.
};

class MultiplayerState : public State
{
  private:
    class ServerSelector
    {
      public:
        EngineData &data;               ///< Engine data used for various purposes, like window size and textures.
        sf::Texture iconTexture;        ///< Texture of the world icon.
        ServerMetadata metadata;        ///< Metadata of the world associated with this selector.
        sf::RectangleShape container;   ///< Container shape for the world selector.
        sf::RectangleShape icon;        ///< Icon shape representing the world.
        std::unique_ptr<sf::Text> name; ///< Name of the world displayed.
        std::unique_ptr<sf::Text> description; ///< Description of the world displayed.

        bool selected; ///< Flag indicating if the world is selected.

        /**
         * @brief Constructs a ServerSelector object.
         * @param data Engine data reference for accessing resources like textures and fonts.
         * @param metadata Metadata associated with the world.
         * @param y_position The vertical position for the server selector in the menu.
         */
        ServerSelector(EngineData &data, ServerMetadata metadata, const float y_position)
            : data(data), metadata(metadata), selected(false)
        {
            container.setSize(sf::Vector2f(gui::percent(data.vm->size.x, 60.f), gui::percent(data.vm->size.y, 18.f)));
            container.setPosition(sf::Vector2f(static_cast<int>(data.vm->size.x / 2.f - container.getSize().x / 2.f),
                                               static_cast<int>(y_position)));
            container.setFillColor(sf::Color::Transparent);
            container.setOutlineColor(sf::Color::White);

            iconTexture = data.activeResourcePack->getTexture("Default");
            icon.setSize(sf::Vector2f(sf::Vector2f(container.getSize().y, container.getSize().y)));
            icon.setPosition(container.getPosition());
            icon.setTexture(&iconTexture);

            name = std::make_unique<sf::Text>(data.activeResourcePack->fonts.at("Bold"), metadata.serverName,
                                              gui::charSize(*data.vm, 100));

            name->setPosition(sf::Vector2f(
                static_cast<int>(icon.getPosition().x + icon.getSize().x + gui::percent(data.vm->size.x, 1.f)),
                static_cast<int>(icon.getPosition().y + gui::percent(data.vm->size.y, 1.f))));

            name->setFillColor(sf::Color::White);

            description = std::make_unique<sf::Text>(data.activeResourcePack->fonts.at("Regular"),
                                                     "\"" + metadata.serverDescription + "\"" + "\nGame Version: " +
                                                         metadata.gameVersion + "\nStatus: " + metadata.status,
                                                     gui::charSize(*data.vm, 100));

            description->setPosition(
                sf::Vector2f(static_cast<int>(name->getPosition().x),
                             static_cast<int>(name->getPosition().y + gui::percent(data.vm->size.y, 5.f))));

            description->setFillColor(sf::Color(200, 200, 200, 255));
        }

        /**
         * @brief Updates the state of the server selector based on mouse interaction.
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
         * @brief Renders the server selector to the target.
         * @param target The render target where the server selector will be drawn.
         */
        inline void render(sf::RenderTarget &target)
        {
            target.draw(container);
            target.draw(icon);
            target.draw(*name);
            target.draw(*description);
        }

        /**
         * @brief Gets the position of the server selector container.
         * @return Position of the server selector container.
         */
        inline const sf::Vector2f getPosition() const
        {
            return container.getPosition();
        }

        /**
         * @brief Gets the size of the server selector container.
         * @return Size of the server selector container.
         */
        inline const sf::Vector2f getSize() const
        {
            return container.getSize();
        }

        /**
         * @brief Checks if the server selector is selected.
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

    sf::RectangleShape background;
    sf::RectangleShape header;
    sf::RectangleShape footer;

    sf::RectangleShape buttonContainer;

    std::map<std::string, std::unique_ptr<gui::TextButton>> buttons;

    std::unique_ptr<gui::ScrollableContainer> serverSelectorsList;
    std::vector<std::shared_ptr<ServerSelector>> serverSelectors;
    std::optional<std::shared_ptr<ServerSelector>> selectedServer;

    void initGUI();

    void initServerSelectors();

  public:
    MultiplayerState(EngineData &data);

    ~MultiplayerState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);

    void updateGUI(const float &dt);

    void renderGUI(sf::RenderTarget &target);
};