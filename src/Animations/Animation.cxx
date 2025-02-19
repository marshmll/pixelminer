#include "Animations/Animation.hxx"
#include "stdafx.hxx"

Animation::Animation(sf::Sprite &sprite, sf::Texture &sprite_sheet, const std::int32_t &frametime_as_ms,
                     const sf::Vector2u frame_size, const sf::Vector2u &start_frame_index,
                     const sf::Vector2u end_frame_index, const bool boomerang)
    : sprite(sprite), spriteSheet(sprite_sheet), frametimeAsMs(frametime_as_ms), frameSize(frame_size),
      boomerang(boomerang), currentDirection(AnimationDirection::Fowards)
{
    sprite.setTexture(spriteSheet);
    sprite.setTextureRect(startFrameRect);

    startFrameRect = sf::IntRect(sf::Vector2i(start_frame_index.x * frame_size.x, start_frame_index.y * frame_size.y),
                                 sf::Vector2i(frame_size));

    endFrameRect = sf::IntRect(sf::Vector2i(end_frame_index.x * frame_size.x, end_frame_index.y * frame_size.y),
                               sf::Vector2i(frame_size));

    currentFrameRect = startFrameRect;

    animationClock.restart();
}

Animation::~Animation() = default;

void Animation::play()
{
    if (animationClock.getElapsedTime().asMilliseconds() >= frametimeAsMs)
    {
        animationClock.restart();

        if (currentDirection == AnimationDirection::Fowards)
        {
            currentFrameRect.position.x += frameSize.x;

            if (currentFrameRect.position.x > endFrameRect.position.x)
            {
                currentFrameRect.position.x = 0;
                currentFrameRect.position.y += frameSize.y;

                if (currentFrameRect.position.y > endFrameRect.position.y)
                {
                    if (boomerang)
                    {
                        currentDirection = AnimationDirection::Backwards;
                        currentFrameRect.position.x = endFrameRect.position.x - frameSize.x;
                        currentFrameRect.position.y = endFrameRect.position.y;
                    }
                    else
                    {
                        currentFrameRect = startFrameRect;
                    }
                }
            }
        }
        else if (currentDirection == AnimationDirection::Backwards)
        {
            currentFrameRect.position.x -= frameSize.x;

            if (currentFrameRect.position.x < startFrameRect.position.x)
            {
                currentFrameRect.position.x = endFrameRect.position.x - frameSize.x;
                currentFrameRect.position.y -= frameSize.y;

                if (currentFrameRect.position.y < 0)
                {
                    if (boomerang)
                    {
                        currentDirection = AnimationDirection::Fowards;
                        currentFrameRect.position.x = startFrameRect.position.x + frameSize.x;
                        currentFrameRect.position.y = startFrameRect.position.y;
                    }
                    else
                    {
                        currentFrameRect = endFrameRect;
                    }
                }
            }
        }
    }

    sprite.setTextureRect(currentFrameRect);
}

void Animation::pause()
{
    animationClock.stop();
}

void Animation::resume()
{
    animationClock.start();
}

const bool Animation::isRunning()
{
    return animationClock.isRunning();
}

void Animation::reset()
{
    currentDirection = AnimationDirection::Fowards;
    currentFrameRect = startFrameRect;
}