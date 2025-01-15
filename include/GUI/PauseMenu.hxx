#pragma once

#include "GUI/TextButton.hxx"
#include "Map/Map.hxx"
#include "States/State.hxx"

namespace gui
{
class PauseMenu
{
  private:
    sf::RectangleShape background;
    sf::RectangleShape buttonContainer;
    std::map<std::string, std::unique_ptr<gui::TextButton>> buttons;
    bool active;
    EngineData &data;
    Map &map;

  public:
    PauseMenu(EngineData &data, Map &map);
    ~PauseMenu();

    void update(const float &dt, const sf::Vector2f &mouse_pos);

    void render(sf::RenderTarget &target);

    const bool &isActive() const;

    void toggleActive();

    void setActive(const bool &active);
};
} // namespace gui
