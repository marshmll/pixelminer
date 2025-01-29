#include "GUI/ScrollableContainer.hxx"
#include "stdafx.hxx"

using namespace gui;

ScrollableContainer::ScrollableContainer(const sf::VideoMode &vm, const sf::Vector2f &size,
                                         const sf::Vector2f &position, const float &scrollbar_width,
                                         const sf::Color &scrollbar_color)
{
    // Container
    container.setSize(size);
    container.setPosition(position);
    container.setFillColor(sf::Color::Transparent);

    // Scrollbar
    scrollBar.setSize(sf::Vector2f(scrollbar_width, container.getSize().y / 4.f));
    scrollBar.setPosition(sf::Vector2f(container.getPosition().x + container.getSize().x, container.getPosition().y));
    scrollBar.setFillColor(scrollbar_color);

    // View
    scrollView.setSize(
        sf::Vector2f(container.getSize().x + 2.f, container.getSize().y + 2.f)); // Match view size to container
    scrollView.setCenter(sf::Vector2f(
        static_cast<int>(container.getPosition().x + container.getSize().x / 2.f),
        static_cast<int>(container.getPosition().y + container.getSize().y / 2.f))); // Center it on the container

    // Viewport mapping (to proportional rendering to a part of the screen)
    scrollView.setViewport(sf::FloatRect({container.getPosition().x / static_cast<float>(vm.size.x),
                                          container.getPosition().y / static_cast<float>(vm.size.y)},
                                         {container.getSize().x / static_cast<float>(vm.size.x),
                                          container.getSize().y / static_cast<float>(vm.size.y)}));
}

ScrollableContainer::~ScrollableContainer()
{
}

void ScrollableContainer::update(const float &dt, const sf::Vector2f &mouse_pos, std::optional<sf::Event> &event,
                                 sf::Event::MouseWheelScrolled &mouse_data)
{
    if (event->is<sf::Event::MouseWheelScrolled>())
    {
        scrollView.move(sf::Vector2f(0.f, 800.f * dt * mouse_data.delta));
    }
}

void ScrollableContainer::render(sf::RenderTarget &target)
{
    target.draw(scrollBar);
}

const sf::Vector2f ScrollableContainer::getSize() const
{
    return container.getSize();
}

const sf::Vector2f ScrollableContainer::getPosition() const
{
    return container.getPosition();
}

sf::View &ScrollableContainer::getView()
{
    return scrollView;
}