#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <string>

#include "molecule.hpp"

class simulator_manager
{
public:
   static void simulationStart(const std::string&, molecule&,const sf::Font&,const std::vector<atom>&);
};

#endif
