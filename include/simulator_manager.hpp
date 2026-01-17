#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <string>

#include "chemical_database.hpp"
#include "audio_manager.hpp"
#include "molecule.hpp"
#include "ion.hpp"
#include "leaderboard.hpp"

struct resolution
{
    unsigned int width, height;
};

// struct UIMenu
// {
//     sf::Text titleText;
//
//     sf::RectangleShape sandboxButton, triviaButton, leaderboardButton, exitButton, tutorialButton;
//     sf::Text sandboxText, triviaText, leaderboardText, exitText, tutorialText;
//
//     sf::CircleShape helpButton;
//     sf::Text helpButtonText;
//
//     sf::RectangleShape overlayBg;
//     sf::RectangleShape rulesPanel, purposePanel;
//     sf::Text rulesTitle, rulesBody;
//     sf::Text purposeTitle, purposeBody;
//     sf::Text closeInfo;
//
//     explicit UIMenu(const sf::Font& font) : titleText(font), sandboxText(font), triviaText(font), leaderboardText(font), exitText(font), tutorialText(font), helpButtonText(font), rulesTitle(font), rulesBody(font), purposeTitle(font),
//                                             purposeBody(font), closeInfo(font){}
// };


class simulator_manager
{
    std::vector<event_observer*> observers;                                 //implementat observer pattern
    void notifySelectedItem() const;

    static simulator_manager instance;

    simulator_manager() = default;
    simulator_manager(const simulator_manager&) = delete;
    simulator_manager& operator=(const simulator_manager&) = delete;
    ~simulator_manager() = default;

    struct UIMenu
    {
        sf::Text titleText;

        sf::RectangleShape sandboxButton, triviaButton, leaderboardButton, exitButton, tutorialButton;
        sf::Text sandboxText, triviaText, leaderboardText, exitText, tutorialText;

        sf::CircleShape helpButton;
        sf::Text helpButtonText;

        sf::RectangleShape overlayBg;
        sf::RectangleShape rulesPanel, purposePanel;
        sf::Text rulesTitle, rulesBody;
        sf::Text purposeTitle, purposeBody;
        sf::Text closeInfo;

        explicit UIMenu(const sf::Font& font) : titleText(font), sandboxText(font), triviaText(font), leaderboardText(font), exitText(font), tutorialText(font), helpButtonText(font), rulesTitle(font), rulesBody(font), purposeTitle(font),
                                                purposeBody(font), closeInfo(font){}
    };

    static void UIMenuSetup(const resolution&, const sf::Font&, UIMenu&);
    static void UIMenuDraw(sf::RenderWindow&, UIMenu&, sf::Sprite&, bool);

    static std::vector<std::shared_ptr<atom>> setupPalette(const std::vector<atom>& templateAtoms, const std::vector<ion>& templateIons);
     void handleLeftClick(sf::Vector2f mousePos, molecule& thisMolecule, const std::vector<std::shared_ptr<atom>>& atomPalette, int& selectedTemplateIndex, int& draggedAtomIndex, bool& isDragging, sf::Vector2f& dragOffset) const;
     void handleRightClick(sf::Vector2f mousePos, molecule& thisMolecule, int& selectedAtomIndex, audio_manager& audio) const;
    static void drawPalette(sf::RenderWindow& window, const std::vector<std::shared_ptr<atom>>& atomPalette, const sf::RectangleShape& menuBg, const sf::RectangleShape& selectionBox);
    static void scoreSaveMode(sf::RenderWindow&, int, const sf::Font&, const molecule&, leaderboard&);


public:
    void addObserver(event_observer* obs);

    static resolution newResolution();

    void simulation(const resolution&);
    void sandboxMode(sf::RenderWindow&, molecule&, const sf::Font&, const std::vector<atom>&, const std::vector<ion>&, chemical_database&, audio_manager&);
    void triviaMode(sf::RenderWindow&, molecule&, const sf::Font&, const std::vector<atom>&, const std::vector<ion>&, const chemical_database&, audio_manager&);
    void leaderboardMode(sf::RenderWindow&, const sf::Font&, audio_manager&);
    void tutorialMode(sf::RenderWindow& window, const sf::Font& font, audio_manager& audio) const;
    static simulator_manager& getInstance();
};

#endif
