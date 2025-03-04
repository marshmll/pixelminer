/**
 * @file ScrollableContainer.hxx
 * @brief Declares the gui::ScrollableContainer class for creating containers that can
 * hold content larger that its visible area.
 */

#pragma once

#include "GUI/Utils.hxx"

namespace gui
{
    /**
     * @class ScrollableContainer
     * @brief A class that represents a scrollable container with a scrollbar.
     *
     * The ScrollableContainer class provides the functionality for a container that can hold
     * content larger than its visible area. It includes a scrollbar to allow the user to scroll
     * through the content, with features for both mouse wheel scrolling and manual dragging of the scrollbar.
     */
    class ScrollableContainer
    {
      private:
        sf::RectangleShape container; ///< The rectangle shape representing the container's visual bounds.
        sf::RectangleShape scrollBar; ///< The rectangle shape representing the scrollbar.
        sf::View scrollView;          ///< The view used to render the content inside the container.
        float maxScrollDelta; ///< The maximum scrollable distance, controlling how far the view can be scrolled.

        bool scrollBarLock;       ///< Whether the scrollbar is being dragged by the user.
        bool scrollBarVisibility; ///< Whether the scrollbar is visible or hidden.

      public:
        /**
         * @brief Constructs a ScrollableContainer with the specified properties.
         *
         * This constructor initializes the scrollable container with a given size, position, maximum scroll
         * delta, and scrollbar width. It also sets up the associated scrollbar and view.
         *
         * @param vm The video mode, used for viewport mapping.
         * @param size The size of the scrollable container.
         * @param position The position of the scrollable container.
         * @param max_scroll_delta The maximum scrollable distance inside the container.
         * @param scrollbar_width The width of the scrollbar.
         * @param scrollbar_color The color of the scrollbar (default is white).
         * @param container_color The color of the contianer (default is transparent).
         */
        ScrollableContainer(const sf::VideoMode &vm, const sf::Vector2f &size, const sf::Vector2f &position,
                            const float &max_scroll_delta, const float &scrollbar_width,
                            const sf::Color &scrollbar_color = sf::Color::White,
                            const sf::Color &container_color = sf::Color::Transparent);

        /** Destructor for ScrollableContainer */
        ~ScrollableContainer();

        /**
         * @brief Updates the state of the scrollable container based on user input.
         *
         * This method processes mouse movement, wheel scrolling, and scrollbar dragging.
         * It updates the scroll position based on user interactions.
         *
         * @param dt The delta time since the last update.
         * @param mouse_pos The current position of the mouse.
         * @param mouse_data An optional mouse_data that might contain mouse wheel scroll information.
         */
        void update(const float &dt, const sf::Vector2f &mouse_pos,
                    std::optional<sf::Event::MouseWheelScrolled> &mouse_data);

        /**
         * @brief Renders the scrollable container to the given render target.
         *
         * This method draws the scrollbar and the container onto the specified render target.
         *
         * @param target The render target to which the container will be drawn.
         */
        void render(sf::RenderTarget &target);

        /**
         * @brief Gets the size of the scrollable container.
         *
         * This method returns the size of the container.
         *
         * @return The size of the container.
         */
        const sf::Vector2f getSize() const;

        /**
         * @brief Gets the position of the scrollable container.
         *
         * This method returns the position of the container.
         *
         * @return The position of the container.
         */
        const sf::Vector2f getPosition() const;

        /**
         * @brief Gets the view associated with the scrollable container.
         *
         * This method provides access to the view used to render the content inside the container.
         *
         * @return The view associated with the scrollable container.
         */
        sf::View &getView();

        /**
         * @brief Checks if the scrollbar is being dragged.
         *
         * This method returns whether the scrollbar is currently locked for dragging by the user.
         *
         * @return A constant reference to the scroll lock state.
         */
        const bool &isScrollLocked() const;

        /**
         * @brief Sets the maximum scroll delta for the container.
         *
         * This method sets the maximum scrollable distance inside the container.
         *
         * @param delta The new maximum scroll delta.
         */
        void setMaxScrollDelta(const float &delta);

        /**
         * @brief Sets the maximum scroll delta based on the last element's position and tolerance.
         *
         * This method calculates the maximum scroll delta based on the position of the last visible element.
         * The tolerance adjusts how far the user can scroll past the last element.
         *
         * @param last_element_bottom_y The Y-coordinate of the last element's bottom edge.
         * @param tolerance The tolerance applied to the scroll delta calculation.
         */
        void setMaxScrollDelta(const float &last_element_bottom_y, const float &tolerance);

        /**
         * @brief Sets the scroll position of the view as a percentage.
         *
         * This method sets the scroll position of the view based on the provided percentage.
         *
         * @param percent The percentage of scroll to apply (0 to 1).
         */
        void setViewScrollPercent(const float &percent);

        /**
         * @brief Sets the scrollbar position as a percentage.
         *
         * This method adjusts the scrollbar position based on the provided percentage.
         *
         * @param percent The percentage of scrollbar scroll to apply (0 to 1).
         */
        void setScrollBarPercent(const float &percent);

        /**
         * @brief Sets the visibility of the scrollbar.
         *
         * This method controls whether the scrollbar is visible or hidden.
         *
         * @param visibility The visibility state of the scrollbar.
         */
        void setScrollBarVisibility(const bool &visibility);

        /**
         * @brief Sets the scroll position as a percentage for both the view and the scrollbar.
         *
         * This method adjusts both the scroll view and scrollbar positions based on the provided percentage.
         *
         * @param percent The percentage of scroll to apply (0 to 1).
         */
        void setScrollPercent(const float &percent);

        /**
         * @brief Scrolls the container to the bottom.
         *
         * This method scrolls the view and scrollbar to the bottom of the container, making the last content visible.
         */
        void scrollToBottom();
    };
} // namespace gui
