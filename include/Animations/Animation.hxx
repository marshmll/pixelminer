/**
 * @file Animation.hxx
 * @brief Declares the Animation class for handling sprite animations.
 */

#pragma once

/**
 * @enum AnimationDirection
 * @brief Specifies the direction of animation playback.
 */
enum AnimationDirection
{
    Fowards = 0, ///< Animation progresses forward
    Backwards    ///< Animation progresses backward
};

/**
 * @class Animation
 * @brief Manages sprite animations using a sprite sheet.
 */
class Animation
{
  private:
    sf::Sprite &sprite;         ///< Reference to the sprite being animated
    sf::Texture &spriteSheet;   ///< Reference to the texture containing animation frames
    std::int32_t frametimeAsMs; ///< Time per frame in milliseconds
    sf::Vector2u frameSize;     ///< Dimensions of each animation frame

    sf::IntRect startFrameRect;   ///< Rect defining the first frame of the animation
    sf::IntRect endFrameRect;     ///< Rect defining the last frame of the animation
    sf::IntRect currentFrameRect; ///< Rect defining the current frame being displayed

    bool boomerang;                      ///< Indicates if the animation should play in reverse after finishing
    AnimationDirection currentDirection; ///< Current animation playback direction

    sf::Clock animationClock; ///< Clock to manage animation timing

  public:
    /**
     * @brief Constructs an Animation object.
     * @param sprite Reference to the sprite to animate.
     * @param sprite_sheet Reference to the texture containing animation frames.
     * @param frametime_as_ms Frame duration in milliseconds.
     * @param frame_size Dimensions of each animation frame.
     * @param start_frame_index Starting frame index (x, y) in the sprite sheet.
     * @param end_frame_index Ending frame index (x, y) in the sprite sheet.
     * @param boomerang If true, animation plays backward after reaching the last frame.
     */
    Animation(sf::Sprite &sprite, sf::Texture &sprite_sheet, const std::int32_t &frametime_as_ms,
              const sf::Vector2u frame_size, const sf::Vector2u &start_frame_index, const sf::Vector2u end_frame_index,
              const bool boomerang = false);

    /**
     * @brief Destructor.
     */
    ~Animation();

    /**
     * @brief Starts or resumes the animation.
     */
    void play();

    /**
     * @brief Pauses the animation.
     */
    void pause();

    /**
     * @brief Resumes the animation from where it was paused.
     */
    void resume();

    /**
     * @brief Checks if the animation is currently running.
     * @return True if the animation is running, false otherwise.
     */
    const bool isRunning();

    /**
     * @brief Resets the animation to the initial frame.
     */
    void reset();
};
