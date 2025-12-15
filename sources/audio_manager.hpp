#ifndef OOP_AUDIO_MANAGER_HPP
#define OOP_AUDIO_MANAGER_HPP

#include "SFML/Audio.hpp"
#include <map>
#include <string>

class audio_manager
{
    sf::Music mainMusic;
    std::map<std::string, sf::SoundBuffer> rawSounds;
    std::map<std::string, std::shared_ptr<sf::Sound>> sounds;

public:
    void playMusic(const std::string&);
    void loadSound(const std::string&, const std::string&);
    void playSound(const std::string&);
};


#endif