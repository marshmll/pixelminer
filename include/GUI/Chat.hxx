/**
 * @file Chat.hxx
 * @brief Declares the gui::Chat class for managing in-game messaging and commands functionalities.
 */

#pragma once

#include "GUI/Utils.hxx"

namespace gui
{
/**
 * @class Chat
 * @brief Represents a chat interface with messaging functionality.
 *
 * Handles the display of chat messages, user input, and message scrolling.
 */
class Chat
{
  private:
    /**
     * @class Message
     * @brief Represents an individual message in the chat.
     *
     * Contains message content, appearance, and opacity for fading effects.
     */
    class Message
    {
      public:
        sf::RectangleShape container;   ///< The background container of the message.
        std::unique_ptr<sf::Text> text; ///< The text object for the message content.
        float opacity;                  ///< The opacity of the message for fading effects.

        /**
         * @brief Constructs a Message object.
         *
         * Initializes the message container and text with author, content, position, and other properties.
         *
         * @param author The author of the message.
         * @param content The content of the message.
         * @param position The position where the message will be displayed.
         * @param width The width of the message container.
         * @param padding Padding around the text inside the message container.
         * @param font The font used for the message text.
         * @param char_size The character size of the text.
         */
        Message(const std::string &author, const std::string &content, const sf::Vector2f &position, const float &width,
                const float &padding, sf::Font &font, const unsigned int &char_size)
            : opacity(100.f)
        {
            container.setSize(sf::Vector2f(width, 0.f));
            container.setPosition(position);
            container.setFillColor(sf::Color(0, 0, 0, 80));

            sf::String text_content = author.empty() ? content : "[" + author + "]: " + content;

            text = std::make_unique<sf::Text>(font, text_content, char_size);
            text->setPosition(sf::Vector2f(static_cast<int>(container.getPosition().x + padding),
                                           static_cast<int>(container.getPosition().y + padding)));
            text->setFillColor(sf::Color::White);

            // Break string to fit container
            for (int i = 0; i < text_content.getSize(); i++)
            {
                if (text->findCharacterPos(i).x >= container.getPosition().x + container.getSize().x - padding)
                {
                    sf::String str = text->getString();
                    str.insert(i, "\n");
                    text->setString(str);
                }
            }

            container.setSize(sf::Vector2f(container.getSize().x, text->getGlobalBounds().size.y + padding));
            container.move(sf::Vector2f(0.f, padding));
        }

        /**
         * @brief Destructor for the Message class.
         */
        ~Message() = default;

        /**
         * @brief Updates the message opacity and other properties.
         *
         * @param dt Delta time for frame updates.
         * @param with_fade Whether to apply fading effect.
         */
        inline void update(const float &dt, const bool &with_fade = true)
        {
            if (with_fade)
            {
                opacity -= 10.f * dt;

                if (opacity <= 0.f)
                    opacity = 0.f;
            }
        }

        /**
         * @brief Renders the message to the target.
         *
         * @param target The render target.
         * @param with_fade Whether to apply fading effect during rendering.
         */
        inline void render(sf::RenderTarget &target, const bool &with_fade = true)
        {
            float tmp = opacity;

            if (!with_fade)
                opacity = 100.f;

            updateTextOpacity();
            opacity = tmp;

            target.draw(*text);
        }

        /**
         * @brief Updates the opacity of the message text.
         */
        inline void updateTextOpacity()
        {
            text->setFillColor(sf::Color(text->getFillColor().r, text->getFillColor().g, text->getFillColor().b,
                                         static_cast<uint8_t>(255 * (opacity / 100.f))));
        }

        /**
         * @brief Gets the current opacity of the message.
         *
         * @return The opacity value.
         */
        inline const float &getOpacity() const
        {
            return opacity;
        }

        /**
         * @brief Gets the position of the message.
         *
         * @return The position of the message container.
         */
        inline const sf::Vector2f getPosition() const
        {
            return container.getPosition();
        }

        /**
         * @brief Gets the size of the message container.
         *
         * @return The size of the message container.
         */
        inline const sf::Vector2f getSize() const
        {
            return container.getSize();
        }

        /**
         * @brief Sets the opacity of the message.
         *
         * @param opacity The new opacity value.
         */
        inline void setOpacity(const float &opacity)
        {
            this->opacity = opacity;
        }

        /**
         * @brief Checks if the message is visible based on its opacity.
         *
         * @return True if the message is visible (opacity > 0), false otherwise.
         */
        inline const bool isVisible() const
        {
            return opacity > 0.f;
        }
    };

    std::string thisAuthor; ///< The author of the chat (used for sending messages).

    std::unique_ptr<gui::ScrollableContainer> chatContainer; ///< Container holding the chat messages.
    std::deque<std::unique_ptr<Message>> messages;           ///< The list of chat messages.
    std::unique_ptr<gui::Input> input;                       ///< Input field for typing new messages.

    sf::Font &font;    ///< Font used for the text in the chat.
    sf::VideoMode &vm; ///< Video mode to manage the screen size.

    bool active; ///< Whether the chat is active (visible).

  public:
    /**
     * @brief Constructs a Chat object.
     *
     * Initializes the chat interface with the author, size, position, font, and video mode.
     *
     * @param this_author The name of the chat author.
     * @param size The size of the chat window.
     * @param position The position of the chat window.
     * @param font The font used for chat messages.
     * @param vm The video mode for the display.
     */
    Chat(const std::string &this_author, const sf::Vector2f &size, const sf::Vector2f &position, sf::Font &font,
         sf::VideoMode &vm);

    /**
     * @brief Destructor for the Chat class.
     *
     * Cleans up the chat interface and messages.
     */
    ~Chat();

    /**
     * @brief Updates the chat interface and its components.
     *
     * Handles message fading, input updates, and container interactions.
     *
     * @param dt Delta time for frame updates.
     * @param mouse_pos The position of the mouse cursor.
     * @param event Optional event data for user input.
     * @param mouse_data Data related to mouse wheel scrolling.
     */
    void update(const float &dt, const sf::Vector2f &mouse_pos, std::optional<sf::Event> &event,
                sf::Event::MouseWheelScrolled &mouse_data);

    /**
     * @brief Renders the chat interface and messages.
     *
     * Draws the chat container, input field, and all messages.
     *
     * @param target The render target to draw the chat to.
     */
    void render(sf::RenderTarget &target);

    /**
     * @brief Displays a new message in the chat.
     *
     * Adds a new message from a specific author with content to the chat.
     *
     * @param author The author of the message.
     * @param content The content of the message.
     */
    void displayMessage(const std::string &author, const std::string &content);

    /**
     * @brief Checks if the chat is active (visible).
     *
     * @return True if the chat is active, false otherwise.
     */
    const bool &isActive() const;

    /**
     * @brief Sets the active state of the chat.
     *
     * @param active The new active state of the chat.
     */
    void setActive(const bool &active);

    /**
     * @brief Sends a message from the input field to the chat.
     */
    void sendMessageFromInput();
};
} // namespace gui
