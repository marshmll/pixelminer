#pragma once

#include "GUI/GUI.hxx"

namespace gui
{
class Chat
{
  private:
    class Message
    {
      public:
        sf::RectangleShape container;
        std::unique_ptr<sf::Text> text;
        float opacity;

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

        ~Message() = default;

        inline void update(const float &dt, const bool &with_fade = true)
        {
            if (with_fade)
            {
                opacity -= 5.f * dt;

                if (opacity <= 0.f)
                    opacity = 0.f;
            }
        }

        inline void render(sf::RenderTarget &target, const bool &with_fade = true)
        {
            float tmp = opacity;

            if (!with_fade)
                opacity = 100.f;

            updateTextOpacity();
            opacity = tmp;

            target.draw(*text);
        }

        inline void updateTextOpacity()
        {
            text->setFillColor(sf::Color(text->getFillColor().r, text->getFillColor().g, text->getFillColor().b,
                                         static_cast<uint8_t>(255 * (opacity / 100.f))));
        }

        inline const float &getOpacity() const
        {
            return opacity;
        }

        inline const sf::Vector2f getPosition() const
        {
            return container.getPosition();
        }

        inline const sf::Vector2f getSize() const
        {
            return container.getSize();
        }

        inline void setOpacity(const float &opacity)
        {
            this->opacity = opacity;
        }

        inline const bool isVisible() const
        {
            return opacity > 0.f;
        }
    };

    std::unique_ptr<gui::ScrollableContainer> chatContainer;
    std::deque<std::unique_ptr<Message>> messages;

    sf::Font &font;
    sf::VideoMode &vm;

    bool active;

  public:
    Chat(const sf::Vector2f &size, const sf::Vector2f &position, sf::Font &font, sf::VideoMode &vm);

    ~Chat();

    void update(const float &dt, const sf::Vector2f &mouse_pos, std::optional<sf::Event> &event,
                sf::Event::MouseWheelScrolled &mouse_data);

    void render(sf::RenderTarget &target);

    void displayMessage(const std::string &author, const std::string &content);

    const bool &isActive() const;

    void setActive(const bool &active);
};
} // namespace gui