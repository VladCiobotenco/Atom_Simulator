#include "audio_manager.hpp"
#include "exceptions.hpp"

void audio_manager::playMusic(const std::string& fileName)
{
    if (!mainMusic.openFromFile(fileName))
        throw resourceMissingException(fileName);

    mainMusic.setLooping(true);
    mainMusic.setVolume(50.f);
    mainMusic.play();
}

void audio_manager::loadSound(const std::string& name, const std::string& fileName)
{
    sf::SoundBuffer temporarySound;
    if (!temporarySound.loadFromFile(fileName))
        throw resourceMissingException(fileName);

    rawSounds[name] = temporarySound;
    auto soundPtr = std::make_shared<sf::Sound>(temporarySound);
    soundPtr->setBuffer(rawSounds[name]);
    sounds[name] = std::move(soundPtr);
}

void audio_manager::playSound(const std::string& name)
{
    if (!sounds.contains(name))
        throw resourceMissingException("Name of sound: " + name);

    sounds[name]->play();
}


