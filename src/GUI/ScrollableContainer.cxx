#include "GUI/ScrollableContainer.hxx"
#include "stdafx.hxx"

using namespace gui;

ScrollableContainer::ScrollableContainer(const sf::VideoMode &vm, const sf::Vector2f &size,
                                         const sf::Vector2f &position, const float &max_scroll_delta,
                                         const float &scrollbar_width, const sf::Color &scrollbar_color)
    : maxScrollDelta(max_scroll_delta), scrollBarLock(false), scrollBarVisibility(true)
{
    // Cache container position and size
    container.setSize(size);
    container.setPosition(position);

    const float containerCenterX = container.getPosition().x + container.getSize().x / 2.f;
    const float containerCenterY = container.getPosition().y + container.getSize().y / 2.f;

    // Scrollbar
    scrollBar.setSize(sf::Vector2f(scrollbar_width, container.getSize().y / 4.f));
    scrollBar.setPosition(sf::Vector2f(container.getPosition().x + container.getSize().x + scrollBar.getSize().x / 2.f,
                                       container.getPosition().y));
    scrollBar.setFillColor(scrollbar_color);

    // View
    scrollView.setSize(
        sf::Vector2f(container.getSize().x + 2.f, container.getSize().y + 2.f)); // Match view size to container
    scrollView.setCenter(sf::Vector2f(containerCenterX, containerCenterY));      // Center it on the container

    // Viewport mapping (to proportional rendering to a part of the screen)
    const float viewportLeft = container.getPosition().x / static_cast<float>(vm.size.x);
    const float viewportTop = container.getPosition().y / static_cast<float>(vm.size.y);
    const float viewportWidth = container.getSize().x / static_cast<float>(vm.size.x);
    const float viewportHeight = container.getSize().y / static_cast<float>(vm.size.y);
    scrollView.setViewport(sf::FloatRect({viewportLeft, viewportTop}, {viewportWidth, viewportHeight}));
}

ScrollableContainer::~ScrollableContainer() = default;

void ScrollableContainer::update(const float &dt, const sf::Vector2f &mouse_pos, std::optional<sf::Event> &event)
{
    if (maxScrollDelta <= 0.f)
        return;

    const float containerTop = container.getPosition().y;
    const float containerBottom = containerTop + container.getSize().y;
    const float scrollBarHeight = scrollBar.getSize().y;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && scrollBarLock)
    {
        float newScrollBarY = mouse_pos.y - scrollBarHeight / 2.f;

        // Clamp scrollbar position within container bounds
        newScrollBarY = std::clamp(newScrollBarY, containerTop, containerBottom - scrollBarHeight);
        scrollBar.setPosition(sf::Vector2f(scrollBar.getPosition().x, newScrollBarY));

        // Calculate scroll percentage based on scrollbar position
        float percent = (newScrollBarY - containerTop) / (containerBottom - containerTop - scrollBarHeight);
        setScrollPercent(percent);
    }
    else if (scrollBar.getGlobalBounds().contains(mouse_pos) && scrollBarVisibility)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            scrollBarLock = true;
    }
    else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        scrollBarLock = false;
    }

    if (!scrollBarLock)
    {
        if (const auto *mouse_data = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            float scrollDelta = 800.f * dt * -mouse_data->delta;
            float newViewCenterY = scrollView.getCenter().y + scrollDelta;

            // Clamp view center within scrollable bounds
            newViewCenterY = std::clamp(newViewCenterY, containerTop + container.getSize().y / 2.f,
                                        containerTop + container.getSize().y / 2.f + maxScrollDelta);
            scrollView.setCenter(sf::Vector2f(scrollView.getCenter().x, static_cast<int>(newViewCenterY)));

            // Update scrollbar position based on view position
            float scrollPercent = (newViewCenterY - (containerTop + container.getSize().y / 2.f)) / maxScrollDelta;
            setScrollPercent(scrollPercent);
        }
    }
}

void ScrollableContainer::render(sf::RenderTarget &target)
{
    // target.draw(container);

    if (maxScrollDelta > 0.f && scrollBarVisibility)
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

const bool &ScrollableContainer::isScrollLocked() const
{
    return scrollBarLock;
}

void ScrollableContainer::setMaxScrollDelta(const float &delta)
{
    maxScrollDelta = delta;
}

void ScrollableContainer::setMaxScrollDelta(const float &last_element_bottom_y, const float &tolerance)
{
    const float containerBottom = container.getPosition().y + container.getSize().y;

    // No scrolling if content does not overflow container
    if (last_element_bottom_y <= containerBottom)
        maxScrollDelta = 0.f;
    else
        // Delta = last_element_bottom_y - container_top_y - view_size_y + tolerance
        maxScrollDelta = last_element_bottom_y - container.getPosition().y - scrollView.getSize().y + tolerance;
}

void ScrollableContainer::setViewScrollPercent(const float &percent)
{
    float newViewCenterY = (container.getPosition().y + container.getSize().y / 2.f) + maxScrollDelta * percent;
    scrollView.setCenter(sf::Vector2f(scrollView.getCenter().x, newViewCenterY));
}

void ScrollableContainer::setScrollBarPercent(const float &percent)
{
    float newScrollBarY = container.getPosition().y + (container.getSize().y - scrollBar.getSize().y) * percent;
    scrollBar.setPosition(sf::Vector2f(scrollBar.getPosition().x, newScrollBarY));
}

void ScrollableContainer::setScrollBarVisibility(const bool &visibility)
{
    scrollBarVisibility = visibility;
}

void ScrollableContainer::setScrollPercent(const float &percent)
{
    setViewScrollPercent(percent);
    setScrollBarPercent(percent);
}

void ScrollableContainer::scrollToBottom()
{
    setScrollPercent(1.f);
}