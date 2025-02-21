#pragma once

#include "Tools/Logger.hxx"

class SoundFunctionality
{
  private:
    Logger logger;

    std::unordered_map<std::string, sf::SoundBuffer> &soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds;

  public:
    SoundFunctionality(std::unordered_map<std::string, sf::SoundBuffer> &sound_buffers);

    ~SoundFunctionality();

    void addSound(const std::string sound_key, const std::string sound_buffer_key, const float &volume);

    void playSound(const std::string sound_key);

    const sf::SoundSource::Status getSoundStatus(const std::string sound_key);

    const bool exists(const std::string sound_key);
};
