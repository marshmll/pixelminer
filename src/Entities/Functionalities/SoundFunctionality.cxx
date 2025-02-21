#include "Entities/Functionalities/SoundFunctionality.hxx"

SoundFunctionality::SoundFunctionality(std::unordered_map<std::string, sf::SoundBuffer> &sound_buffers)
    : logger("SoundFunctionality"), soundBuffers(sound_buffers)
{}

SoundFunctionality::~SoundFunctionality() = default;

void SoundFunctionality::addSound(const std::string sound_key, const std::string sound_buffer_key, const float &volume)
{
    sounds[sound_key] = std::make_unique<sf::Sound>(soundBuffers.at(sound_buffer_key));
    sounds[sound_key]->setVolume(volume);
}

void SoundFunctionality::playSound(const std::string sound_key)
{
    try
    {
        sounds.at(sound_key)->play();
    }
    catch (std::out_of_range &)
    {
        logger.logError(_("Inexistent sound: ") + sound_key);
    }
}

const sf::Sound::Status SoundFunctionality::getSoundStatus(const std::string sound_key)
{
    try
    {
        return sounds.at(sound_key)->getStatus();
    }
    catch (std::out_of_range &)
    {
        logger.logError(_("Inexistent sound: ") + sound_key);
    }

    return sf::Sound::Status::Stopped; // SHOULD NEVER REACH HERE!
}

const bool SoundFunctionality::exists(const std::string sound_key)
{
    return sounds.count(sound_key) > 0;
}
