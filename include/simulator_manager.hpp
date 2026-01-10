#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <string>

#include "chemical_database.hpp"
#include "audio_manager.hpp"
#include "molecule.hpp"
#include "ion.hpp"
#include "leaderboard.hpp"

struct resolution {
    unsigned int width, height;
};

class simulator_manager
{
    static simulator_manager instance;

    simulator_manager() = default;
    simulator_manager(const simulator_manager&) = delete;
    simulator_manager& operator=(const simulator_manager&) = delete;
    ~simulator_manager() = default;

    static std::vector<std::shared_ptr<atom>> setupPalette(const std::vector<atom>& templateAtoms, const std::vector<ion>& templateIons);
    static void handleLeftClick(sf::Vector2f mousePos, molecule& thisMolecule, const std::vector<std::shared_ptr<atom>>& atomPalette, int& selectedTemplateIndex, int& draggedAtomIndex, bool& isDragging, sf::Vector2f& dragOffset, audio_manager& audio);
    static void handleRightClick(sf::Vector2f mousePos, molecule& thisMolecule, int& selectedAtomIndex, audio_manager& audio);
    static void drawPalette(sf::RenderWindow& window, const std::vector<std::shared_ptr<atom>>& atomPalette, const sf::RectangleShape& menuBg, const sf::RectangleShape& selectionBox);
    static void scoreSaveMode(sf::RenderWindow&, int, sf::Font, const molecule&, leaderboard&);


public:
    resolution newResolution();

    void simulation(const resolution&);
    static void sandboxMode(sf::RenderWindow&, molecule&, const sf::Font&, const std::vector<atom>&, const std::vector<ion>&, chemical_database&, audio_manager&);
    static void triviaMode(sf::RenderWindow&, molecule&, const sf::Font&, const std::vector<atom>&, const std::vector<ion>&, const chemical_database&, audio_manager&);
    static void leaderboardMode(sf::RenderWindow&, const sf::Font&, audio_manager&);
    static void tutorialMode(sf::RenderWindow& window, const sf::Font& font, audio_manager& audio);
    static simulator_manager& getInstance();
};

#endif
