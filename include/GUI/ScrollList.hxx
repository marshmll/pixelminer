#pragma once

namespace gui
{
class ScrollList
{
  private:
    sf::RectangleShape container;
    sf::RectangleShape scrollBar;
    sf::View scrollView;

  public:
    ScrollList(const sf::VideoMode &vm, const sf::Vector2f &size, const sf::Vector2f &position,
               const float &scrollbar_width, const sf::Color &scrollbar_color = sf::Color::White);

    ~ScrollList();

    void update(const float &dt, const sf::Vector2f &mouse_pos);

    void render(sf::RenderTarget &target);

    const sf::Vector2f getSize() const;

    const sf::Vector2f getPosition() const;

    sf::View &getView();
};
} // namespace gui