#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>
#include <string>

#include "atom.hpp"
#include "ion.hpp"

std::vector<atom> readAtomsFromJson(const std::string&);
std::vector<ion> readIonsFromJson(const std::string&, sf::Font);

#endif