#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <string>

#include "chemical_database.hpp"
#include "audio_manager.hpp"
#include "molecule.hpp"

class simulator_manager
{
public:
   static void simulationStart(const std::string&, molecule&, const sf::Font&, const std::vector<atom>&, const std::vector<ion>&, chemical_database&, audio_manager&);
};

#endif
