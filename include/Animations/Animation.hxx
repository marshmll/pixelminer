#pragma once

enum AnimationDirection
{
    Fowards = 0,
    Backwards,
};

class Animation
{
  private:
    sf::Sprite &sprite;
    sf::Texture &spriteSheet;
    std::int32_t frametimeAsMs;
    sf::Vector2u frameSize;

    sf::IntRect startFrameRect;
    sf::IntRect endFrameRect;
    sf::IntRect currentFrameRect;

    bool boomerang;
    AnimationDirection currentDirection;

    sf::Clock animationClock;

  public:
    Animation(sf::Sprite &sprite, sf::Texture &sprite_sheet, const std::int32_t &frametime_as_ms,
              const sf::Vector2u frame_size, const sf::Vector2u &start_frame_index, const sf::Vector2u end_frame_index,
              const bool boomerang = false);

    ~Animation();

    void play();

    void pause();

    void resume();

    const bool isRunning();

    void reset();
};