#include <string>
#include <SFML/Graphics.hpp>
#include <optional>

#include "../include/atom.hpp"
#include "../include/bond.hpp"
#include "../include/molecule.hpp"
#include "../include/ion.hpp"
#include "../include/input.hpp"

#include "../include/simulator_manager.hpp"
#include "../include/audio_manager.hpp"

#include <iostream>

#include "../include/double_bond.hpp"
#include "../include/exceptions.hpp"
#include "../include/leaderboard.hpp"
#include "../include/single_bond.hpp"

void simulator_manager::simulation() {
    sf::Font font;
    if (!font.openFromFile("../assets/Roboto-VariableFont_wdth,wght.ttf"))
        throw resourceMissingException("../assets/Roboto-VariableFont_wdth,wght.ttf");

    sf::Texture texture;
    if (!texture.loadFromFile("../assets/menu-background.png"))
        throw resourceMissingException("../assets/menu-background.png");

    sf::Sprite background(texture);
    float scaleX = 1000.f/740.f;
    float scaleY = 600.f/495.f;
    background.setScale({scaleX, scaleY});
    background.setPosition({0, 0});

    std::vector<atom> inputAtoms = input<atom>::read("../data/atoms.json");
    std::vector<ion> inputIons = input<ion>::read("../data/ions.json", font);

    chemical_database database;
    database.loadIntoDatabase("../data/elements.json");

    audio_manager audio;
    audio.playMusic("../assets/MainMusic.ogg");
    audio.loadSound("selectie","../assets/click-selectare.wav");
    audio.loadSound("stergere","../assets/click-stergere.wav");
    audio.loadSound("corect","../assets/trivia-corect.wav");
    audio.loadSound("gresit","../assets/trivia-gresit.wav");

    sf::RenderWindow window(sf::VideoMode({1000, 600}), "Atom Simulator");

    sf::Text titleText(font);
    titleText.setCharacterSize(50);
    titleText.setString("Atom Simulator");
    titleText.setFillColor(sf::Color::Black);
    titleText.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.position.x + titleBounds.size.x / 2, titleBounds.position.y + titleBounds.size.y / 2});
    titleText.setPosition({500.f, 50.f});

    sf::Vector2f buttonSize(300.f, 60.f);
    float buttonX = 500.f;
    float startY = 250.f;
    float gapY = 100.f;

    sf::RectangleShape sandboxButton(buttonSize);
    sandboxButton.setOrigin({buttonSize.x / 2, buttonSize.y / 2});
    sandboxButton.setPosition({buttonX, startY});
    sandboxButton.setFillColor(sf::Color(192, 192, 192));
    sandboxButton.setOutlineThickness(2.f);
    sandboxButton.setOutlineColor(sf::Color(100, 100, 100));
    sf::Text sandboxText(font);
    sandboxText.setCharacterSize(24);
    sandboxText.setString("Sandbox mode");
    sf::FloatRect sandboxBounds = sandboxText.getLocalBounds();
    sandboxText.setOrigin({sandboxBounds.position.x + sandboxBounds.size.x/ 2, sandboxBounds.position.y + sandboxBounds.size.y/ 2});
    sandboxText.setPosition({buttonX, startY});

    sf::RectangleShape triviaButton(buttonSize);
    triviaButton.setOrigin({buttonSize.x / 2, buttonSize.y / 2});
    triviaButton.setPosition({buttonX, startY + gapY});
    triviaButton.setFillColor(sf::Color(192, 192, 192));
    triviaButton.setOutlineThickness(2.f);
    triviaButton.setOutlineColor(sf::Color(100, 100, 100));
    sf::Text triviaText(font);
    triviaText.setCharacterSize(24);
    triviaText.setString("Trivia mode");
    sf::FloatRect triviaBounds = triviaText.getLocalBounds();
    triviaText.setOrigin({triviaBounds.position.x + triviaBounds.size.x/ 2, triviaBounds.position.y + triviaBounds.size.y / 2});
    triviaText.setPosition({buttonX, startY + gapY});

    sf::RectangleShape leaderboardButton(buttonSize);
    leaderboardButton.setOrigin({buttonSize.x / 2, buttonSize.y / 2});
    leaderboardButton.setPosition({buttonX, startY + gapY*2});
    leaderboardButton.setFillColor(sf::Color(70, 70, 70));
    leaderboardButton.setOutlineThickness(2.f);
    leaderboardButton.setOutlineColor(sf::Color(100, 100, 100));
    sf::Text leaderboardText(font);
    leaderboardText.setCharacterSize(24);
    leaderboardText.setString("Leaderboard");
    sf::FloatRect leaderboardBounds = leaderboardText.getLocalBounds();
    leaderboardText.setOrigin({leaderboardBounds.position.x + leaderboardBounds.size.x/2, leaderboardBounds.position.y + leaderboardBounds.size.y/2});
    leaderboardText.setPosition({buttonX, startY + gapY*2});

    sf::RectangleShape exitButton(buttonSize);
    exitButton.setOrigin({buttonSize.x / 2, buttonSize.y / 2});
    exitButton.setPosition({buttonX, startY + gapY * 3});
    exitButton.setFillColor(sf::Color(192, 192, 192));
    exitButton.setOutlineThickness(2.f);
    exitButton.setOutlineColor(sf::Color(100, 100, 100));
    sf::Text exitText(font);
    exitText.setCharacterSize(24);
    exitText.setString("Exit");
    sf::FloatRect exitBounds = exitText.getLocalBounds();
    exitText.setOrigin({exitBounds.position.x + exitBounds.size.x / 2, exitBounds.position.y + exitBounds.size.y / 2});
    exitText.setPosition({buttonX, startY + gapY * 3});

    sf::Vector2f tutorialButtonSize(100.f, 40.f);

    sf::RectangleShape tutorialButton(tutorialButtonSize);
    tutorialButton.setOrigin({tutorialButtonSize.x / 2.f, tutorialButtonSize.y / 2.f});
    tutorialButton.setPosition({920.f, 50.f});
    tutorialButton.setFillColor(sf::Color(70, 70, 70));
    tutorialButton.setOutlineThickness(2.f);
    tutorialButton.setOutlineColor(sf::Color(100, 100, 100));
    sf::Text tutorialText(font, "Tutorial", 20);
    sf::FloatRect tutBounds = tutorialText.getLocalBounds();
    tutorialText.setOrigin({tutBounds.position.x + tutBounds.size.x/2.f, tutBounds.position.y + tutBounds.size.y/2.f});
    tutorialText.setPosition(tutorialButton.getPosition());

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }
            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePress->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mousePress->position);
                    if (sandboxButton.getGlobalBounds().contains(mousePos))
                    {
                        audio.playSound("selectie");
                        molecule testMolecule("sandboxMolecule");
                        sandboxMode(window, testMolecule, font, inputAtoms, inputIons, database, audio);
                        testMolecule.removeEntities();
                    }
                    else if (triviaButton.getGlobalBounds().contains(mousePos))
                    {
                        audio.playSound("selectie");
                        molecule testMolecule("triviaMolecule");
                        triviaMode(window, testMolecule, font, inputAtoms, inputIons, database, audio);
                        testMolecule.removeEntities();
                    }
                    else if (leaderboardButton.getGlobalBounds().contains(mousePos)) {
                        audio.playSound("selectie");
                        leaderboardMode(window,font,audio);
                    }
                    else if (tutorialButton.getGlobalBounds().contains(mousePos)) {
                        audio.playSound("selectie");
                        // No molecule needed for tutorial mode
                        tutorialMode(window, font, audio);
                    }
                    else if (exitButton.getGlobalBounds().contains(mousePos))
                    {
                        window.close();
                        return;
                    }
                }
            }
            if (!window.isOpen())
                break;

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            auto updateHover = [&](sf::RectangleShape& button)
            {
                if (button.getGlobalBounds().contains(mousePos))
                    button.setFillColor(sf::Color(100, 100, 100));
                else
                    button.setFillColor(sf::Color(70, 70, 70));
            };
            updateHover(sandboxButton);
            updateHover(triviaButton);
            updateHover(leaderboardButton);
            updateHover(exitButton);
            updateHover(tutorialButton);

            window.clear(sf::Color(30, 30, 30));

            window.draw(background);

            window.draw(titleText);

            window.draw(sandboxButton);
            window.draw(sandboxText);

            window.draw(triviaButton);
            window.draw(triviaText);

            window.draw(leaderboardButton);
            window.draw(leaderboardText);

            window.draw(exitButton);
            window.draw(exitText);

            window.draw(tutorialButton);
            window.draw(tutorialText);

            window.display();
        }
    }
}

void simulator_manager::sandboxMode(sf::RenderWindow& window, molecule& thisMolecule, const sf::Font& font, const std::vector<atom>& templateAtoms, const std::vector<ion>& templateIons, chemical_database& database, audio_manager& audio)
{
    /// Crearea unei palete de atomi in zona de menu
    std::vector<std::shared_ptr<atom>>  atomPalette = setupPalette(templateAtoms, templateIons);

    /// Infoboxul pentru atomi cand dam hover
    sf::Text infoText(font);
    infoText.setCharacterSize(14);
    infoText.setFillColor(sf::Color::Black);
    sf::RectangleShape infoBox;
    infoBox.setFillColor(sf::Color::White);
    infoBox.setOutlineColor(sf::Color::Black);
    infoBox.setOutlineThickness(1.f);
    infoBox.setSize({150.f, 25.f});
    bool infoBoxVisibility = false;

    sf::RectangleShape atomMenuBackground({200,600});
    atomMenuBackground.setFillColor(sf::Color(50, 50, 50));

    sf::Text dashboardText(font);
    dashboardText.setCharacterSize(14);
    dashboardText.setFillColor(sf::Color::White);
    dashboardText.setPosition({10.f, 560.f});
    sf::RectangleShape dashboardBox;
    dashboardBox.setFillColor(sf::Color(70, 70, 70));
    dashboardBox.setOutlineColor(sf::Color::Black);
    dashboardBox.setOutlineThickness(1.f);
    dashboardBox.setSize({200.f, 100.f});
    dashboardBox.setPosition({0.f,550.f});

    sf::Text formulaText(font);
    formulaText.setCharacterSize(24);
    formulaText.setFillColor(sf::Color::White);
    formulaText.setStyle(sf::Text::Bold);
    formulaText.setPosition({220.f, 20.f});
    sf::RectangleShape formulaBox;
    formulaBox.setFillColor(sf::Color(70, 70, 70));
    formulaBox.setOutlineThickness(1.f);
    formulaBox.setSize({200.f, 40.f});  // Dimensiunea acestui box se va mari in functie de masa atomica
    formulaBox.setPosition({220.f, 20.f});

    sf::RectangleShape selectionBox;
    selectionBox.setFillColor(sf::Color::Transparent);
    selectionBox.setOutlineColor(sf::Color::Yellow);
    selectionBox.setOutlineThickness(3.f);

    sf::FloatRect workArea({200, 0}, {800, 600});

    int selectedTemplateIndex = 0;
    int draggedAtomIndex = -1; //folosit pentru dragging
    int selectedAtomIndex = -1; //folosit pentru bonding
    bool isDragging = false;
    sf::Vector2f dragOffset;

    while (window.isOpen())
    {
        sf::FloatRect templateBounds = atomPalette[selectedTemplateIndex]->getBounds();
        selectionBox.setSize({templateBounds.size.x + 10.f, templateBounds.size.y + 10.f});
        selectionBox.setOrigin({5.f, 5.f});
        selectionBox.setPosition(atomPalette[selectedTemplateIndex]->getAtomPosition() - sf::Vector2f(templateBounds.size.x/2, templateBounds.size.y/2));

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>())
            {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if (keyPressed->code==sf::Keyboard::Key::Escape)
                    return;

                if (keyPressed->code==sf::Keyboard::Key::Delete)
                    if (selectedAtomIndex !=-1 )
                    {
                        thisMolecule.removeAtom(selectedAtomIndex);
                        selectedAtomIndex = -1;
                        audio.playSound("stergere");
                    }

                if (keyPressed->code==sf::Keyboard::Key::R)
                    thisMolecule.removeEntities();
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonPressed->position);

                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                    handleLeftClick(mousePos,thisMolecule,atomPalette, selectedTemplateIndex, draggedAtomIndex, isDragging, dragOffset, audio);

                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                    handleRightClick(mousePos, thisMolecule, selectedAtomIndex, audio);

            }

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                const sf::Event::MouseButtonReleased* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>();
                /// Se muta atomul
                if (mouseButtonReleased->button == sf::Mouse::Button::Left && isDragging == true)
                {
                    const auto draggedAtom = thisMolecule.getAtom(draggedAtomIndex);
                    draggedAtom->setAtomThickness(2.f);
                    isDragging=false;
                    draggedAtomIndex=-1;
                }

            }
        }

        /// Atomul este mutat
        if (isDragging && draggedAtomIndex != -1)
        {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            const auto currentAtom = thisMolecule.getAtom(draggedAtomIndex);
            if (currentAtom)
            {
                currentAtom->setPosition(worldPos + dragOffset);
                currentAtom->restrictAtomToBounds(workArea);
                thisMolecule.updateBondsPositions();
                infoBoxVisibility=false;
            }
        }

        /// Se afiseaza numele atomului cu hover
        if (!isDragging)
        {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            int hoveredAtomIndex=thisMolecule.findAtomAtPosition(worldPos);
            if (hoveredAtomIndex != -1)
            {
                infoBoxVisibility=true;
                const auto hoveredAtom = thisMolecule.getAtom(hoveredAtomIndex);
                std::string info = hoveredAtom->getName() + " (" + hoveredAtom->getSymbol() + ")";
                infoText.setString(info);

                sf::Vector2f infoBoxPosition=worldPos+sf::Vector2f(10.f,10.f);
                infoBox.setPosition(infoBoxPosition);
                infoText.setPosition(infoBoxPosition+sf::Vector2f(40.f,0.f));
            }
            else infoBoxVisibility=false;
        }

        /// Se afiseaza numele atomului selectat din menu
        if (selectedTemplateIndex>=0)
        {
            const auto& selectedMenuAtom=atomPalette[selectedTemplateIndex];
            std::string info = "Selected atom: " + selectedMenuAtom->getName() + " (" + selectedMenuAtom->getSymbol() + ")";
            dashboardText.setString(info);
        }

        std::string currentFormula = thisMolecule.getMolecularFormula();
        if (currentFormula.empty())
            formulaText.setString("Formula: (Empty)");
        else
        {
            std::string outputMoleculeText;
            outputMoleculeText = "Formula: " + currentFormula + "(Masa moleculei = " + std::to_string(thisMolecule.getMoleculeMass()) + ")";
            std::string name = database.searchIntoDatabase(currentFormula);
            if (!name.empty())
                formulaText.setString(outputMoleculeText + " (" + name + ")");
            else formulaText.setString(outputMoleculeText);
        }

        sf::FloatRect textBounds = formulaText.getLocalBounds();
        float paddingX = 10.f;
        float paddingY = 10.f;
        formulaBox.setSize({textBounds.size.x + paddingX * 2, textBounds.size.y + paddingY * 2});
        formulaText.setPosition({formulaBox.getPosition().x + paddingX, formulaBox.getPosition().y + paddingY});


        window.clear(sf::Color(232, 219, 135));
        thisMolecule.draw(window);
        window.draw(atomMenuBackground);
        window.draw(dashboardBox);
        window.draw(dashboardText);
        for (const auto& menuAtom : atomPalette) {
            menuAtom->draw(window);
        }
        window.draw(selectionBox);
        if (infoBoxVisibility)
        {
            window.draw(infoBox);
            window.draw(infoText);
        }
        window.draw(formulaBox);
        window.draw(formulaText);

        window.display();
    }
}

void simulator_manager::triviaMode(sf::RenderWindow& window, molecule& thisMolecule, const sf::Font& font, const std::vector<atom>& templateAtoms, const std::vector<ion>& templateIons, const chemical_database& database, audio_manager& audio)
{
    window.setTitle("Atom Simulator - Trivia Mode");

    thisMolecule.removeEntities();
    int score = 0;

    auto currentTarget = database.getRandomEntry();
    std::string targetFormula = currentTarget.first;
    std::string targetName = currentTarget.second;

    std::vector<std::shared_ptr<atom>> atomPalette = setupPalette(templateAtoms, templateIons);

    sf::Text targetText(font, "Creeaza: " + targetName, 30);
    targetText.setFillColor(sf::Color::White);
    targetText.setPosition({250.f, 20.f});

    sf::Text scoreText(font, "Scor: 0", 30);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition({800.f, 20.f});

    sf::Text feedbackText(font, "", 24);
    feedbackText.setFillColor(sf::Color::Red);
    feedbackText.setPosition({400.f, 550.f});

    sf::RectangleShape submitButton({150.f, 50.f});
    submitButton.setFillColor(sf::Color(0, 150, 0));
    submitButton.setPosition({800.f, 530.f});
    sf::Text submitLabel(font, "SUBMIT", 20);
    sf::FloatRect submitBounds = submitLabel.getLocalBounds();
    submitLabel.setOrigin({submitBounds.size.x/2.f, submitBounds.size.y/2.f});
    submitLabel.setPosition({800.f + 75.f, 530.f + 25.f});

    sf::RectangleShape atomMenuBackground({200.f, 600.f});
    atomMenuBackground.setFillColor(sf::Color(50, 50, 50));

    sf::FloatRect workArea({200.f, 0.f}, {800.f, 600.f});
    sf::RectangleShape selectionBox;
    selectionBox.setFillColor(sf::Color::Transparent);
    selectionBox.setOutlineColor(sf::Color::Yellow);
    selectionBox.setOutlineThickness(3.f);

    int selectedTemplateIndex = 0;
    int draggedAtomIndex = -1;
    int selectedAtomIndex = -1;
    bool isDragging = false;
    sf::Vector2f dragOffset;

    // Variabile pentru timer
    sf::Clock clock;
    bool waitNewMolecule = false;

    // Variabila de leaderboard
    leaderboard board("../data/scores.txt");

    while (window.isOpen())
    {
        if (!atomPalette.empty())
        {
            sf::FloatRect b = atomPalette[selectedTemplateIndex]->getBounds();
            selectionBox.setSize({b.size.x + 10.f, b.size.y + 10.f});
            selectionBox.setOrigin({5.f, 5.f});
            selectionBox.setPosition(atomPalette[selectedTemplateIndex]->getAtomPosition() - sf::Vector2f(b.size.x/2, b.size.y/2));
        }

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }

            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    scoreSaveMode(window, score, font, thisMolecule, board);
                    return;
                }
                if (keyEvent->code == sf::Keyboard::Key::Delete && selectedAtomIndex != -1)
                {
                    thisMolecule.removeAtom(selectedAtomIndex);
                    selectedAtomIndex = -1;
                    audio.playSound("stergere");
                }
                if (keyEvent->code == sf::Keyboard::Key::R)
                    thisMolecule.removeEntities();
            }

            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(mousePress->position);

                if (mousePress->button == sf::Mouse::Button::Left)
                {
                    if (submitButton.getGlobalBounds().contains(mousePos))
                    {
                        if (!waitNewMolecule && thisMolecule.getMolecularFormula() == targetFormula)
                        {
                            score++;
                            scoreText.setString("Scor: " + std::to_string(score));
                            feedbackText.setString("Corect!"); feedbackText.setFillColor(sf::Color::Green);
                            audio.playSound("corect");
                            clock.restart();
                            waitNewMolecule = true;
                        }
                        else
                        {
                            feedbackText.setString("Gresit!"); feedbackText.setFillColor(sf::Color::Red);
                            audio.playSound("gresit");
                        }
                    }
                    else
                        handleLeftClick(mousePos, thisMolecule, atomPalette, selectedTemplateIndex, draggedAtomIndex, isDragging, dragOffset, audio);
                }

                else if(mousePress->button == sf::Mouse::Button::Right)
                    handleRightClick(mousePos, thisMolecule, selectedAtomIndex, audio);
            }

            if (const auto* mouseRel = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseRel->button == sf::Mouse::Button::Left && isDragging) {
                    if (draggedAtomIndex != -1) {
                        auto atom = thisMolecule.getAtom(draggedAtomIndex);
                        if(atom) atom->setAtomThickness(2.f);
                    }
                    isDragging = false;
                    draggedAtomIndex = -1;
                }
            }
        }

        if (isDragging && draggedAtomIndex != -1) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            auto atom = thisMolecule.getAtom(draggedAtomIndex);
            if(atom) {
                atom->setPosition(mousePos + dragOffset);
                atom->restrictAtomToBounds(workArea);
                thisMolecule.updateBondsPositions();
            }
        }

        if (waitNewMolecule == true)
        {
            if (clock.getElapsedTime().asSeconds() > 1.0f)
            {
                waitNewMolecule = false;
                feedbackText.setString("");
                thisMolecule.removeEntities();
                currentTarget = database.getRandomEntry();
                targetFormula = currentTarget.first;
                targetName = currentTarget.second;
                targetText.setString("Creeaza: " + targetName);
            }
        }

        window.clear(sf::Color(40, 44, 52));

        window.draw(targetText);
        window.draw(scoreText);
        window.draw(feedbackText);

        thisMolecule.draw(window);
        drawPalette(window, atomPalette, atomMenuBackground, selectionBox);

        window.draw(submitButton);
        window.draw(submitLabel);

        window.display();
    }
}

void simulator_manager::leaderboardMode(sf::RenderWindow& window, const sf::Font& font, audio_manager& audio)
{
    leaderboard board("../data/scores.txt");

    audio.playMusic("../assets/Leaderboard.ogg");

    sf::Text leaderboardTitle(font, "High Scores", 40);
    leaderboardTitle.setFillColor(sf::Color::Yellow);
    sf::FloatRect leaderboardTitleBounds = leaderboardTitle.getLocalBounds();
    leaderboardTitle.setOrigin({leaderboardTitleBounds.size.x/2.f, leaderboardTitleBounds.size.y/2.f});
    leaderboardTitle.setPosition({500.f, 50.f});

    sf::RectangleShape leaderboardPanel({600.f, 420.f});
    leaderboardPanel.setFillColor(sf::Color(0, 0, 0, 160));
    leaderboardPanel.setOutlineThickness(2.f);
    leaderboardPanel.setOutlineColor(sf::Color(255, 215, 0));
    leaderboardPanel.setOrigin({300.f, 0.f});
    leaderboardPanel.setPosition({500.f, 110.f});

    std::vector<sf::Text> scoreLines;
    const auto& topScores = board.getScores();
    int rank = 1;
    float startY = 140.f;

    for (const auto& entry : topScores)
    {
        std::string line = std::to_string(rank) + ". " + entry.second + "   " + std::to_string(entry.first);
        sf::Text text(font, line, 26);

        if (rank == 1) text.setFillColor(sf::Color(255, 215, 0));
        else if (rank == 2) text.setFillColor(sf::Color(192, 192, 192));
        else if (rank == 3) text.setFillColor(sf::Color(205, 127, 50));
        else text.setFillColor(sf::Color::White);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.size.x / 2.f, 0.f});
        text.setPosition({500.f, startY});

        scoreLines.push_back(text);
        startY += 35.f;
        rank++;
    }

    if (scoreLines.empty()) {
        sf::Text emptyText(font, "No scores yet!", 30);
        sf::FloatRect eb = emptyText.getLocalBounds();
        emptyText.setOrigin({eb.size.x/2.f, eb.size.y/2.f});
        emptyText.setPosition({500.f, 300.f});
        scoreLines.push_back(emptyText);
    }

    sf::Text backText(font, "Press ESC to return", 20);
    backText.setFillColor(sf::Color(150, 150, 150));
    backText.setPosition({20.f, 560.f});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    audio.playMusic("../assets/MainMusic.ogg");
                    return;
                }
            }
        }

        window.clear(sf::Color(20, 20, 30));

        window.draw(leaderboardTitle);
        window.draw(backText);
        window.draw(leaderboardPanel);
        for (const auto& line: scoreLines)
            window.draw(line);

        window.display();
    }
}

void simulator_manager::tutorialMode(sf::RenderWindow& window, const sf::Font& font, audio_manager& audio) {

    audio.playMusic("../assets/TutorialMusic.ogg");

    sf::Texture periodicTableTexture;
    if (!periodicTableTexture.loadFromFile("../assets/periodic-table.jpg")) {
        throw resourceMissingException("../assets/periodic-table.jpg");
    }

    sf::Sprite backgroundSprite(periodicTableTexture);

    float scaleX = window.getSize().x / static_cast<float>(periodicTableTexture.getSize().x);
    float scaleY = window.getSize().y / static_cast<float>(periodicTableTexture.getSize().y);
    backgroundSprite.setScale({scaleX, scaleY});
    backgroundSprite.setPosition({0, 0});

    float bgX = 0;
    float bgY = 0;

    struct TutorialStep {
        std::string infoText;
        sf::FloatRect highlightRect;
    };

    float rawTileW = 120.f;
    float rawTileH = 140.f;

    auto makeRect = [&](float rawX, float rawY) {
        return sf::FloatRect(
            {bgX + (rawX * scaleX), bgY + (rawY * scaleY)},{rawTileW * scaleX, rawTileH * scaleY}
        );
    };

    std::vector<TutorialStep> steps = {
        {"Hidrogen\n...", makeRect(72.f, 250.f) },
        {"Carbon\n...", makeRect(1706.f, 392.f) },
        {"Azot\n...", makeRect(1831.f, 392.f) },
        {"Oxigen\n...", makeRect(1956.f, 392.f) },
        {"Sodiu\n...", makeRect(72.f, 535.f) },
        {"Sulf\n...", makeRect(1956.f, 535.f) },
        {"Clor\n...", makeRect(2083.f, 535.f) }
    };

    int currentStepIndex = 0;

    sf::RectangleShape infoBoxRect({600.f, 120.f});
    infoBoxRect.setFillColor(sf::Color(0, 0, 0, 220));
    infoBoxRect.setOutlineColor(sf::Color::White);
    infoBoxRect.setOutlineThickness(2.f);
    infoBoxRect.setPosition({200.f, 430.f});

    sf::Text infoText(font, "", 20);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({220.f, 440.f});

    sf::RectangleShape nextButton({120.f, 50.f});
    nextButton.setPosition({820.f, 465.f});
    sf::Text nextTxt(font, "Next >", 24);
    nextTxt.setPosition({845.f, 475.f});

    sf::RectangleShape previousButton({120.f, 50.f});
    previousButton.setPosition({60.f, 465.f});
    sf::Text prevTxt(font, "< Prev", 24);
    prevTxt.setPosition({85.f, 475.f});

    sf::Text exitText(font, "Press ESC to return to Menu", 18);
    exitText.setFillColor(sf::Color::Black);
    exitText.setPosition({20.f, 570.f});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    audio.playMusic("../assets/MainMusic.ogg");
                    return;
                }


                if (key->code == sf::Keyboard::Key::Right && static_cast<size_t>(currentStepIndex) < steps.size() - 1)
                {
                    currentStepIndex++;
                    audio.playSound("selectie");
                }
                if (key->code == sf::Keyboard::Key::Left && currentStepIndex > 0)
                {
                    currentStepIndex--;
                    audio.playSound("selectie");
                }
            }

            if (const auto* press = event->getIf<sf::Event::MouseButtonPressed>())
            {
                 if (press->button == sf::Mouse::Button::Left)
                 {
                     sf::Vector2f mousePos = window.mapPixelToCoords(press->position);
                     if(static_cast<size_t>(currentStepIndex) < steps.size() - 1 && nextButton.getGlobalBounds().contains(mousePos))
                     {
                         currentStepIndex++;
                         audio.playSound("selectie");
                     }
                     if (currentStepIndex > 0 && previousButton.getGlobalBounds().contains(mousePos))
                     {
                         currentStepIndex--;
                         audio.playSound("selectie");
                     }
                 }
            }
        }

        infoText.setString(steps[currentStepIndex].infoText);

        if (currentStepIndex==0)
            previousButton.setFillColor(sf::Color(100, 50, 50));
        else
            previousButton.setFillColor(sf::Color(150, 0, 0));

        if (static_cast<size_t>(currentStepIndex) == steps.size()-1)
            nextButton.setFillColor(sf::Color(50, 100, 50));
        else
            nextButton.setFillColor(sf::Color(0, 150, 0));

        sf::RectangleShape highlightBox;
        sf::FloatRect r = steps[currentStepIndex].highlightRect;
        highlightBox.setPosition({r.position.x, r.position.y});
        highlightBox.setSize({r.size.x, r.size.y});
        highlightBox.setFillColor(sf::Color::Transparent);
        highlightBox.setOutlineThickness(4.0f);
        highlightBox.setOutlineColor(sf::Color(0, 255, 255));

        window.clear(sf::Color(30, 30, 40));

        window.draw(backgroundSprite);
        window.draw(highlightBox);

        window.draw(infoBoxRect);
        window.draw(infoText);

        window.draw(previousButton); window.draw(prevTxt);
        window.draw(nextButton); window.draw(nextTxt);

        window.draw(exitText);

        window.display();
    }
}

std::vector<std::shared_ptr<atom>> simulator_manager::setupPalette(const std::vector<atom>& templateAtoms, const std::vector<ion>& templateIons)
{
    std::vector<std::shared_ptr<atom>> palette;
    float currentX = 100.f;
    float currentY = 50.f;

    for (const auto& tmpl : templateAtoms)
    {
        auto btn = std::make_shared<atom>(tmpl);
        btn->setPosition({currentX, currentY});
        palette.push_back(std::move(btn));
        currentY += 75.f;
    }

    for (const auto& tmpl : templateIons)
    {
        auto btn = std::make_shared<ion>(tmpl);
        btn->setPosition({currentX, currentY});
        palette.push_back(std::move(btn));
        currentY += 75.f;
    }

    return palette;
}

void simulator_manager::handleLeftClick(sf::Vector2f mousePos, molecule& thisMolecule, const std::vector<std::shared_ptr<atom>>& atomPalette,
                                        int& selectedTemplateIndex, int& draggedAtomIndex, bool& isDragging, sf::Vector2f& dragOffset, audio_manager& audio)
{
    if (mousePos.x < 200)
    {
        for (size_t i = 0; i < atomPalette.size(); ++i)
        {
            if (atomPalette[i]->getBounds().contains(mousePos))
            {
                selectedTemplateIndex = (int)i;
                audio.playSound("selectie");
            }
        }
    }
    else
    {
        int clickIndex = thisMolecule.findAtomAtPosition(mousePos);

        if (clickIndex != -1)
        {
            audio.playSound("selectie");
            isDragging = true;
            draggedAtomIndex = clickIndex;
            auto atomPtr = thisMolecule.getAtom(draggedAtomIndex);
            if (atomPtr)
            {
                atomPtr->setAtomThickness(5.f);
                dragOffset = atomPtr->getAtomPosition() - mousePos;
            }
        }
        else
        {
            audio.playSound("selectie");
            auto newEntity = atomPalette[selectedTemplateIndex]->clone();

            if (auto atomPtr = std::dynamic_pointer_cast<atom>(newEntity))
                atomPtr->setShowElectrons(true);

            if (auto ionPtr = std::dynamic_pointer_cast<ion>(newEntity))
                thisMolecule.addAtom(ionPtr, mousePos);
            else if (auto atomPtr = std::dynamic_pointer_cast<atom>(newEntity))
                thisMolecule.addAtom(atomPtr, mousePos);
        }
    }
}

void simulator_manager::handleRightClick(sf::Vector2f mousePos, molecule& thisMolecule,int& selectedAtomIndex, audio_manager& audio)
{
    int clickAtomIndex = thisMolecule.findAtomAtPosition(mousePos);

    if (clickAtomIndex != -1)
    {
        if (selectedAtomIndex == -1)
        {
            audio.playSound("selectie");
            selectedAtomIndex = clickAtomIndex;
            if (auto atom = thisMolecule.getAtom(selectedAtomIndex))
                atom->setAtomThickness(5.f);
        }
        else
        {
            int bondIndex = thisMolecule.findBondPosition(selectedAtomIndex, clickAtomIndex);
            int v1 = thisMolecule.checkValenceLaws(clickAtomIndex);
            int v2 = thisMolecule.checkValenceLaws(selectedAtomIndex);

            try
            {
                if (v1 && v2 && bondIndex == -1)
                {
                    audio.playSound("selectie");
                    std::string type = "single_bond";
                    if (v1 >= 4 && v2 >= 4)
                        type = "quad_bond";
                    else if (v1 >= 3 && v2 >= 3)
                        type = "triple_bond";
                    else if (v1 >= 2 && v2 >= 2)
                        type = "double_bond";

                    thisMolecule.addBond(selectedAtomIndex, clickAtomIndex, type);
                    thisMolecule.updateBondsPositions();
                }
            } catch (const atomSimulatorExceptions& e)
            {
                std::cerr << "EROARE: " << e.what() << "\n";
            }

            if (bondIndex != -1)
            {
                thisMolecule.removeBond(bondIndex);
                audio.playSound("stergere");
            }

            if (auto atom = thisMolecule.getAtom(selectedAtomIndex))
                atom->setAtomThickness(2.f);

            selectedAtomIndex = -1;
        }
        return;
    }
    int clickBondIndex = thisMolecule.findBondAtPosition(mousePos);
    if (clickBondIndex != -1)
    {
        auto bondPtr = thisMolecule.getBond(clickBondIndex);
        if (auto db = std::dynamic_pointer_cast<double_bond>(bondPtr))
        {
            if (thisMolecule.checkHydrocarbon())
            {
                db->toggleConfiguration();
                audio.playSound("selectie");
            }
        }

        if (const auto sb = std::dynamic_pointer_cast<single_bond>(bondPtr))
        {
            if (thisMolecule.getNumberOfAtoms() == 2 && thisMolecule.getNumberOfBonds() == 1)
                sb->toggleSpin();
        }
    }
}

void simulator_manager::drawPalette(sf::RenderWindow& window, const std::vector<std::shared_ptr<atom>>& atomPalette, const sf::RectangleShape& menuBg, const sf::RectangleShape& selectionBox)
{
    window.draw(menuBg);
    for (const auto& item : atomPalette)
        item->draw(window);

    window.draw(selectionBox);
}

void simulator_manager::scoreSaveMode(sf::RenderWindow& window, int score, sf::Font font,const molecule& thisMolecule, leaderboard& board)
{
    if (score > 0)
    {
        const float centerX = 500.f;
        const float centerY = 300.f;

        sf::RectangleShape overlay({800.f, 600.f});
        overlay.setFillColor(sf::Color(0, 0, 0, 150));

        sf::RectangleShape inputBox({300.f, 50.f});
        inputBox.setOrigin({150.f, 25.f});
        inputBox.setPosition({centerX, centerY});
        inputBox.setFillColor(sf::Color(50, 50, 50));
        inputBox.setOutlineThickness(2.f);
        inputBox.setOutlineColor(sf::Color::Cyan);

        sf::Text promptText(font, "Introdu numele: ", 24);
        sf::FloatRect promptBounds = promptText.getLocalBounds();
        promptText.setOrigin({promptBounds.position.x + promptBounds.size.x / 2.f, promptBounds.position.y + promptBounds.size.y / 2.f});
        promptText.setPosition({centerX, centerY - 50.f});

        sf::Text nameText(font, "", 24);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition({260.f, 285.f});

        std::string playerName;
        bool nameSubmitted = false;

        while (window.isOpen() && !nameSubmitted)
        {
            while (const std::optional inputEvent = window.pollEvent())
            {
                if (inputEvent->is<sf::Event::Closed>())
                {
                    window.close();
                    return;
                }

                if (const auto* textEvt = inputEvent->getIf<sf::Event::TextEntered>())
                {
                    if (textEvt->unicode == 8)
                    {
                        if (!playerName.empty())
                            playerName.pop_back();
                    }
                    else if (textEvt->unicode == 13) {
                        if (!playerName.empty())
                            nameSubmitted = true;
                    }
                    else if (textEvt->unicode >31 && textEvt->unicode < 128 && playerName.size() < 12)
                        playerName += static_cast<char>(textEvt->unicode);

                    nameText.setString(playerName);

                    sf::FloatRect nb = nameText.getLocalBounds();
                    nameText.setOrigin({nb.position.x + nb.size.x / 2.f, nb.position.y + nb.size.y / 2.f});
                    nameText.setPosition({centerX, centerY});
                }
            }
            window.clear();

            thisMolecule.draw(window);
            window.draw(overlay);
            window.draw(promptText);
            window.draw(inputBox);
            window.draw(nameText);
            window.display();
        }

        board.addScore(playerName, score);
    }
}

simulator_manager &simulator_manager::getInstance()
{
    return instance;
}

simulator_manager simulator_manager::instance;
