#pragma once

#include "GUI/GUI.hxx"
#include "States/GameState.hxx"
#include "States/State.hxx"
#include "Tools/Date.hxx"
#include "Tools/JSON.hxx"

struct WorldMetadata
{
    std::string worldName;
    std::string folderName;
    unsigned long long creationDate;
    unsigned long long lastPlayed;
    std::string gameVersion;
    std::string difficulty;
};

class WorldSelectionMenuState : public State
{
  private:
    class WorldSelector
    {
      public:
        EngineData &data;
        sf::Texture iconTexture;
        WorldMetadata metadata;
        sf::RectangleShape container;
        sf::RectangleShape icon;
        std::unique_ptr<sf::Text> name;
        std::unique_ptr<sf::Text> description;

        bool selected;

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

        inline void render(sf::RenderTarget &target)
        {
            target.draw(container);
            target.draw(icon);
            target.draw(*name);
            target.draw(*description);
        }

        inline const sf::Vector2f getPosition() const
        {
            return container.getPosition();
        }

        inline const sf::Vector2f getSize() const
        {
            return container.getSize();
        }

        inline const bool isSelected() const
        {
            return selected;
        }

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

    std::vector<std::shared_ptr<WorldSelector>> worldSelectors;
    std::unique_ptr<gui::ScrollableContainer> worldSelectorsList;

    std::optional<std::shared_ptr<WorldSelector>> selectedWorld;

    void initGUI();

    void initWorldSelectors();

  public:
    WorldSelectionMenuState(EngineData &data);

    ~WorldSelectionMenuState();

    void update(const float &dt);

    void render(sf::RenderTarget &target);

    void updateGUI(const float &dt);

    void renderGUI(sf::RenderTarget &target);
};