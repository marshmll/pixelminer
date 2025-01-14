#pragma once

#include "GUI/GUI.hxx"

static std::unordered_map<sf::Keyboard::Key, char> keyMap = {
    {sf::Keyboard::Key::A, 'a'},          {sf::Keyboard::Key::B, 'b'},           {sf::Keyboard::Key::C, 'c'},
    {sf::Keyboard::Key::D, 'd'},          {sf::Keyboard::Key::E, 'e'},           {sf::Keyboard::Key::F, 'f'},
    {sf::Keyboard::Key::G, 'g'},          {sf::Keyboard::Key::H, 'h'},           {sf::Keyboard::Key::I, 'i'},
    {sf::Keyboard::Key::J, 'j'},          {sf::Keyboard::Key::K, 'k'},           {sf::Keyboard::Key::L, 'l'},
    {sf::Keyboard::Key::M, 'm'},          {sf::Keyboard::Key::N, 'n'},           {sf::Keyboard::Key::O, 'o'},
    {sf::Keyboard::Key::P, 'p'},          {sf::Keyboard::Key::Q, 'q'},           {sf::Keyboard::Key::R, 'r'},
    {sf::Keyboard::Key::S, 's'},          {sf::Keyboard::Key::T, 't'},           {sf::Keyboard::Key::U, 'u'},
    {sf::Keyboard::Key::V, 'v'},          {sf::Keyboard::Key::W, 'w'},           {sf::Keyboard::Key::X, 'x'},
    {sf::Keyboard::Key::Y, 'y'},          {sf::Keyboard::Key::Z, 'z'},           {sf::Keyboard::Key::Num0, '0'},
    {sf::Keyboard::Key::Num1, '1'},       {sf::Keyboard::Key::Num2, '2'},        {sf::Keyboard::Key::Num3, '3'},
    {sf::Keyboard::Key::Num4, '4'},       {sf::Keyboard::Key::Num5, '5'},        {sf::Keyboard::Key::Num6, '6'},
    {sf::Keyboard::Key::Num7, '7'},       {sf::Keyboard::Key::Num8, '8'},        {sf::Keyboard::Key::Num9, '9'},
    {sf::Keyboard::Key::Space, ' '},      {sf::Keyboard::Key::Period, '.'},      {sf::Keyboard::Key::Comma, ','},
    {sf::Keyboard::Key::Semicolon, ';'},  {sf::Keyboard::Key::Apostrophe, '\''}, {sf::Keyboard::Key::Slash, '/'},
    {sf::Keyboard::Key::Backslash, '\\'}, {sf::Keyboard::Key::LBracket, '['},    {sf::Keyboard::Key::RBracket, ']'}};

namespace gui
{
class Input
{
  private:
    sf::RectangleShape body;

    std::unique_ptr<sf::Text> label;
    std::unique_ptr<sf::Text> value;

    unsigned int charSize;
    float padding;

    sf::RectangleShape blinkerCursor;
    float cursorTimer;
    float cursorTimerMax;

    sf::Clock keyTimer;

    sf::Keyboard::Key lastKeyPressed;

    bool repeat;

    void toggleCursorVisibility();

    void handleKeyPress(char32_t c);

    void handleBackspace();

    void handleTab();

  public:
    Input(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &body_color, sf::Font &font,
          const unsigned int &char_size, const float &padding, const float &outline_thickness,
          const sf::Color outline_color, const std::string label = "");
    ~Input();

    void update(const float &dt, sf::Vector2f mouse_pos, std::optional<sf::Event> &event);

    void render(sf::RenderTarget &target);

    const std::string getValue() const;
};
} // namespace gui