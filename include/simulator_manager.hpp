#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <string>

#include "chemical_database.hpp"
#include "audio_manager.hpp"
#include "molecule.hpp"

class simulator_manager
{
   static simulator_manager instance;

   simulator_manager() = default;
   simulator_manager(const simulator_manager&) = delete;
   simulator_manager& operator=(const simulator_manager&) = delete;
   ~simulator_manager() = default;

public:
   static void simulation();
   static void sandboxMode(sf::RenderWindow&, molecule&, const sf::Font&, const std::vector<atom>&, const std::vector<ion>&, chemical_database&, audio_manager&);
   static void triviaMode(sf::RenderWindow&, molecule&, const sf::Font&, const std::vector<atom>&, const std::vector<ion>&, const chemical_database&, audio_manager&);

   static simulator_manager& getInstance();


};

#endif
