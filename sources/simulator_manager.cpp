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

void simulator_manager::simulation(const resolution& rez) {

    observers.clear();

    sf::Font font;
    if (!font.openFromFile("Roboto-VariableFont_wdth,wght.ttf"))
        throw resourceMissingException("Roboto-VariableFont_wdth,wght.ttf");

    sf::Texture texture;
    if (!texture.loadFromFile("menu-background.png"))
        throw resourceMissingException("menu-background.png");

    sf::Sprite background(texture);
    float scaleX = rez.width/740.f;
    float scaleY = rez.height/495.f;
    background.setScale({scaleX, scaleY});
    background.setPosition({0, 0});

    std::vector<atom> inputAtoms = input<atom>::read("../data/atoms.json");
    std::vector<ion> inputIons = input<ion>::read("../data/ions.json", font);

    chemical_database database;
    database.loadIntoDatabase("../data/elements.json");

    auto audio = std::make_shared<audio_manager>();
    audio->playMusic("MainMusic.ogg");
    audio->loadSound("selectie","click-selectare.wav");
    audio->loadSound("stergere","click-stergere.wav");
    audio->loadSound("corect","trivia-corect.wav");
    audio->loadSound("gresit","trivia-gresit.wav");

    addObserver(audio);

    sf::RenderWindow window(sf::VideoMode({rez.width, rez.height}), "Atom Simulator");

    UIMenu UI(font);
    UIMenuSetup(rez, font, UI);

    bool showHelp = false;

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
                    if (showHelp)
                        showHelp = false;
                    else
                    {
                        sf::Vector2f mousePos = window.mapPixelToCoords(mousePress->position);
                        if (UI.sandboxButton.getGlobalBounds().contains(mousePos))
                        {
                            notifyEvent("selectie");
                            molecule testMolecule("sandboxMolecule");
                            sandboxMode(window, testMolecule, font, inputAtoms, inputIons, database);
                            testMolecule.removeEntities();
                        }
                        else if (UI.triviaButton.getGlobalBounds().contains(mousePos))
                        {
                            notifyEvent("selectie");
                            molecule testMolecule("triviaMolecule");
                            triviaMode(window, testMolecule, font, inputAtoms, inputIons, database);
                            testMolecule.removeEntities();
                        }
                        else if (UI.leaderboardButton.getGlobalBounds().contains(mousePos)) {
                            notifyEvent("selectie");
                            leaderboardMode(window,font,audio);
                        }
                        else if (UI.tutorialButton.getGlobalBounds().contains(mousePos)) {
                            notifyEvent("selectie");
                            tutorialMode(window, font, audio);
                        }
                        else if (UI.helpButton.getGlobalBounds().contains(mousePos))
                        {
                            showHelp = true;
                            notifyEvent("selectie");
                        }
                        else if (UI.exitButton.getGlobalBounds().contains(mousePos))
                        {
                            window.close();
                            return;
                        }
                    }
                }
            }
            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPress->code == sf::Keyboard::Key::Escape && showHelp)
                {
                    showHelp = false;
                    notifyEvent("selectie");
                }
            }

            if (!window.isOpen())
                break;
        }

        UIMenuDraw(window,UI,background,showHelp);
    }
}

void simulator_manager::sandboxMode(sf::RenderWindow& window, molecule& thisMolecule, const sf::Font& font, const std::vector<atom>& templateAtoms, const std::vector<ion>& templateIons, chemical_database& database) const {
    window.setTitle("Atom Simulator - Sandbox Mode");

    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
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

    sf::RectangleShape atomMenuBackground({200.f,winH});
    atomMenuBackground.setFillColor(sf::Color(50, 50, 50));

    sf::Text dashboardText(font);
    dashboardText.setCharacterSize(14);
    dashboardText.setFillColor(sf::Color::White);
    dashboardText.setPosition({10.f, winH - 40.f});
    sf::RectangleShape dashboardBox;
    dashboardBox.setFillColor(sf::Color(70, 70, 70));
    dashboardBox.setOutlineColor(sf::Color::Black);
    dashboardBox.setOutlineThickness(1.f);
    dashboardBox.setSize({200.f, 100.f});
    dashboardBox.setPosition({0.f, winH - 50.f});

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

    float btnX = winW - 80.f;
    float btnY = winH - 80.f;

    sf::CircleShape playButtonCircle(30.f);
    playButtonCircle.setPosition({btnX, btnY});
    playButtonCircle.setFillColor(sf::Color(50, 50, 50));
    playButtonCircle.setOutlineThickness(2.f);
    playButtonCircle.setOutlineColor(sf::Color::Black);

    sf::ConvexShape playIcon;
    playIcon.setPointCount(3);
    playIcon.setPoint(0, {0.f, 0.f});
    playIcon.setPoint(1, {0.f, 20.f});
    playIcon.setPoint(2, {18.f, 10.f});
    playIcon.setFillColor(sf::Color::Green);
    playIcon.setPosition({btnX + 22.f, btnY + 20.f});

    sf::RectangleShape pauseBar1({6.f, 20.f});
    pauseBar1.setFillColor(sf::Color::Red);
    pauseBar1.setPosition({btnX + 18.f, btnY + 20.f});

    sf::RectangleShape pauseBar2({6.f, 20.f});
    pauseBar2.setFillColor(sf::Color::Red);
    pauseBar2.setPosition({btnX + 36.f, btnY + 20.f});

    sf::Text feedbackText(font);
    feedbackText.setCharacterSize(16);
    feedbackText.setFillColor(sf::Color::Red);
    sf::Clock feedbackClock;
    bool showFeedback = false;

    sf::FloatRect workArea({200.f, 0.f}, {winW - 200.f, winH});

    int selectedTemplateIndex = 0;
    int draggedAtomIndex = -1; //folosit pentru dragging
    int selectedAtomIndex = -1; //folosit pentru bonding
    bool isDragging = false;
    bool isSpinning = false;
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
                {
                    notifyEvent("selectie");
                    return;
                }


                if (keyPressed->code==sf::Keyboard::Key::Delete)
                    if (selectedAtomIndex !=-1 )
                    {
                        thisMolecule.removeAtom(selectedAtomIndex);
                        selectedAtomIndex = -1;
                        notifyEvent("stergere");
                    }

                if (keyPressed->code==sf::Keyboard::Key::R)
                    thisMolecule.removeEntities();
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonPressed->position);

                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    if (playButtonCircle.getGlobalBounds().contains(mousePos)) {
                        if (thisMolecule.checkSimpleMolecule()) {
                            isSpinning = !isSpinning;
                            notifyEvent("selectie");
                            thisMolecule.updateSpin(isSpinning);
                        }
                        else {
                            notifyEvent("gresit");
                            feedbackText.setString("Doar legaturi simple!");
                            sf::FloatRect fb = feedbackText.getLocalBounds();
                            feedbackText.setPosition({btnX - fb.size.x - 10.f, btnY + 20.f});
                            showFeedback = true;
                            feedbackClock.restart();
                        }

                    }
                    else
                        handleLeftClick(mousePos,thisMolecule,atomPalette, selectedTemplateIndex, draggedAtomIndex, isDragging, dragOffset);
                }


                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                    handleRightClick(mousePos, thisMolecule, selectedAtomIndex);

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
            std::string info = "Atom selectat: " + selectedMenuAtom->getName() + " (" + selectedMenuAtom->getSymbol() + ")";
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
        window.draw(playButtonCircle);
        if (!isSpinning)
            window.draw(playIcon);
        else
        {
            window.draw(pauseBar1);
            window.draw(pauseBar2);
        }

        if (showFeedback) {
            if (feedbackClock.getElapsedTime().asSeconds() > 2.f)
                showFeedback = false;
            else
                window.draw(feedbackText);
        }

        window.display();
    }
}

void simulator_manager::triviaMode(sf::RenderWindow& window, molecule& thisMolecule, const sf::Font& font, const std::vector<atom>& templateAtoms, const std::vector<ion>& templateIons, const chemical_database& database) const {
    window.setTitle("Atom Simulator - Trivia Mode");

    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

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
    scoreText.setPosition({winW - 150.f, 20.f});

    sf::Text feedbackText(font, "", 24);
    feedbackText.setFillColor(sf::Color::Red);
    feedbackText.setPosition({200.f + (winW - 200.f)/2.f - 50.f, winH - 120.f});

    sf::RectangleShape submitButton({150.f, 50.f});
    submitButton.setFillColor(sf::Color(0, 150, 0));
    submitButton.setPosition({winW - 180.f, winH - 80.f});
    sf::Text submitLabel(font, "SUBMIT", 20);
    sf::FloatRect submitBounds = submitLabel.getLocalBounds();
    submitLabel.setOrigin({submitBounds.size.x/2.f, submitBounds.size.y/2.f});
    submitLabel.setPosition({submitButton.getPosition().x + 75.f, submitButton.getPosition().y + 25.f});

    sf::RectangleShape atomMenuBackground({200.f, winH});
    atomMenuBackground.setFillColor(sf::Color(50, 50, 50));

    sf::FloatRect workArea({200.f, 0.f}, {winW - 200.f, winH});
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
                    notifyEvent("selectie");
                    scoreSaveMode(window, score, font, thisMolecule, board);
                    return;
                }
                if (keyEvent->code == sf::Keyboard::Key::Delete && selectedAtomIndex != -1)
                {
                    thisMolecule.removeAtom(selectedAtomIndex);
                    selectedAtomIndex = -1;
                    notifyEvent("stergere");
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
                            notifyEvent("corect");
                            clock.restart();
                            waitNewMolecule = true;
                        }
                        else
                        {
                            feedbackText.setString("Gresit!"); feedbackText.setFillColor(sf::Color::Red);
                            notifyEvent("gresit");
                        }
                    }
                    else
                        handleLeftClick(mousePos, thisMolecule, atomPalette, selectedTemplateIndex, draggedAtomIndex, isDragging, dragOffset);
                }

                else if(mousePress->button == sf::Mouse::Button::Right)
                    handleRightClick(mousePos, thisMolecule, selectedAtomIndex);
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

void simulator_manager::leaderboardMode(sf::RenderWindow& window, const sf::Font& font, const std::shared_ptr<audio_manager>& audio) const {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    leaderboard board("../data/scores.txt");
    audio->playMusic("../assets/Leaderboard.ogg");

    sf::Text leaderboardTitle(font, "High Scores", 40);
    leaderboardTitle.setFillColor(sf::Color::Yellow);
    sf::FloatRect leaderboardTitleBounds = leaderboardTitle.getLocalBounds();
    leaderboardTitle.setOrigin({leaderboardTitleBounds.size.x/2.f, leaderboardTitleBounds.size.y/2.f});
    leaderboardTitle.setPosition({winW / 2.f, 50.f});

    sf::RectangleShape leaderboardPanel({600.f, 420.f});
    leaderboardPanel.setFillColor(sf::Color(0, 0, 0, 160));
    leaderboardPanel.setOutlineThickness(2.f);
    leaderboardPanel.setOutlineColor(sf::Color(255, 215, 0));
    leaderboardPanel.setOrigin({300.f, 0.f});
    leaderboardPanel.setPosition({winW / 2.f, 110.f});

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
        text.setPosition({winW / 2.f, startY});

        scoreLines.push_back(text);
        startY += 35.f;
        rank++;
    }

    if (scoreLines.empty()) {
        sf::Text emptyText(font, "Niciun scor inregistrat!", 30);
        sf::FloatRect eb = emptyText.getLocalBounds();
        emptyText.setOrigin({eb.size.x/2.f, eb.size.y/2.f});
        emptyText.setPosition({winW / 2.f, 300.f});
        scoreLines.push_back(emptyText);
    }

    sf::Text backText(font, "Apasa ESC pentru a te intoarce", 20);
    backText.setFillColor(sf::Color(150, 150, 150));
    backText.setPosition({20.f, winH - 40.f});

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
                    notifyEvent("selectie");
                    audio->playMusic("../assets/MainMusic.ogg");
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

void simulator_manager::tutorialMode(sf::RenderWindow& window, const sf::Font& font, const std::shared_ptr<audio_manager>& audio) const {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    audio->playMusic("../assets/TutorialMusic.ogg");

    sf::Texture periodicTableTexture;
    if (!periodicTableTexture.loadFromFile("../assets/periodic-table.jpg")) {
        throw resourceMissingException("../assets/periodic-table.jpg");
    }

    sf::Sprite backgroundSprite(periodicTableTexture);

    float scaleX = winW / static_cast<float>(periodicTableTexture.getSize().x);
    float scaleY = winH / static_cast<float>(periodicTableTexture.getSize().y);
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

    auto makeRect = [&](const float rawX, const float rawY) {
        return sf::FloatRect(
            {bgX + (rawX * scaleX), bgY + (rawY * scaleY)},{rawTileW * scaleX, rawTileH * scaleY}
        );
    };

    std::vector<TutorialStep> steps =
    {
        {
            "Hidrogen (H)\n"
            "Cel mai simplu si abundent element din univers.\n"
            "Culoare: ALB\n"
            "Electroni de valenta: 1\n"
            "Poate forma o singura legatura.",
            makeRect(72.f, 250.f)
        },
        {
            "Carbon (C)\n"
            "Elementul fundamental al vietii si chimiei organice.\n"
            "Culoare: NEGRU/GRI\n"
            "Electroni de valenta: 4\n"
            "Poate forma legaturi simple, duble sau triple.",
            makeRect(1706.f, 392.f)
        },
        {
        "Azot (N)\n"
            "Componenta principala a atmosferei (78%).\n"
            "Culoare: ALBASTRU\n"
            "Electroni de valenta: 5\n"
            "Formeaza de obicei 3 legaturi covalente.",
            makeRect(1831.f, 392.f)
        },
        {
        "Oxigen (O)\n"
            "Vital pentru respiratie si procesele de ardere.\n"
            "Culoare: ROSU\n"
            "Electroni de valenta: 6\n"
            "Formeaza de obicei 2 legaturi.",
            makeRect(1956.f, 392.f)
        },
        {
            "Sodiu (Na)\n"
            "Metal alcalin moale si foarte reactiv.\n"
            "Culoare: VIOLET\n"
            "Electroni de valenta: 1\n"
            "Doneaza usor un electron pentru a forma ioni pozitivi.",
            makeRect(72.f, 535.f)
        },
        {
            "Sulf (S)\n"
            "Essential pentru viata, gasit adesea langa vulcani.\n"
            "Culoare: GALBEN\n"
            "Electroni de valenta: 6\n"
            "Are proprietati chimice similare cu Oxigenul.",
            makeRect(1956.f, 535.f)
        },
        {
            "Clor (Cl)\n"
            "Halogen toxic, folosit adesea la dezinfectare.\n"
            "Culoare: VERDE\n"
            "Electroni de valenta: 7\n"
            "Accepta un electron pentru a completa stratul.",
            makeRect(2083.f, 535.f)
        }
    };

    int currentStepIndex = 0;

    sf::Vector2f boxSize(600.f, 160.f);
    float boxX = (winW - boxSize.x) / 2.f;
    float boxY = winH - boxSize.y - 60.f;
    float btnY = boxY + 35.f;

    sf::RectangleShape infoBoxRect(boxSize);
    infoBoxRect.setFillColor(sf::Color(0, 0, 0, 220));
    infoBoxRect.setOutlineColor(sf::Color::White);
    infoBoxRect.setOutlineThickness(2.f);
    infoBoxRect.setPosition({boxX, boxY});

    sf::Text infoText(font, "", 20);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({boxX + 20.f, boxY + 10.f});

    sf::RectangleShape nextButton({120.f, 50.f});
    nextButton.setPosition({winW - 150.f, btnY});
    sf::Text nextTxt(font, "Next >", 24);
    sf::FloatRect nextB = nextTxt.getLocalBounds();
    nextTxt.setOrigin({nextB.position.x + nextB.size.x / 2.f, nextB.position.y + nextB.size.y / 2.f});
    nextTxt.setPosition({nextButton.getPosition().x + nextButton.getSize().x / 2.f, nextButton.getPosition().y + nextButton.getSize().y / 2.f});

    sf::RectangleShape previousButton({120.f, 50.f});
    previousButton.setPosition({30.f, btnY});
    sf::Text prevTxt(font, "< Prev", 24);
    sf::FloatRect prevB = prevTxt.getLocalBounds();
    prevTxt.setOrigin({prevB.position.x + prevB.size.x / 2.f, prevB.position.y + prevB.size.y / 2.f});
    prevTxt.setPosition({previousButton.getPosition().x + previousButton.getSize().x / 2.f, previousButton.getPosition().y + previousButton.getSize().y / 2.f});

    sf::Text exitText(font, "Apasa ESC pentru a te intoarce", 18);
    exitText.setFillColor(sf::Color::Black);
    exitText.setPosition({20.f, winH - 30.f});

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
                    notifyEvent("selectie");
                    audio->playMusic("../assets/MainMusic.ogg");
                    return;
                }


                if (key->code == sf::Keyboard::Key::Right && static_cast<size_t>(currentStepIndex) < steps.size() - 1)
                {
                    currentStepIndex++;
                    notifyEvent("selectie");
                }
                if (key->code == sf::Keyboard::Key::Left && currentStepIndex > 0)
                {
                    currentStepIndex--;
                    notifyEvent("selectie");
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
                         notifyEvent("selectie");
                     }
                     if (currentStepIndex > 0 && previousButton.getGlobalBounds().contains(mousePos))
                     {
                         currentStepIndex--;
                         notifyEvent("selectie");
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

void simulator_manager::notifyEvent(const std::string& eventType) const {
    for (const auto& obs : observers) obs->onEvent(eventType);
}

void simulator_manager::UIMenuSetup(const resolution& rez, const sf::Font& font, UIMenu& ui)
{
    float centerX = rez.width / 2.f;
    float centerY = rez.height / 2.f;
    float topMargin = rez.height * 0.08f;
    const float buttonStartY = rez.height * 0.35f;
    const float buttonGap = rez.height * 0.15f;

    ui.titleText.setFont(font);
    ui.titleText.setCharacterSize(50);
    ui.titleText.setString("Atom Simulator");
    ui.titleText.setFillColor(sf::Color::Black);
    ui.titleText.setStyle(sf::Text::Bold);
    const sf::FloatRect tb = ui.titleText.getLocalBounds();
    ui.titleText.setOrigin({tb.position.x + tb.size.x / 2.f, tb.position.y + tb.size.y / 2.f});
    ui.titleText.setPosition({centerX, topMargin});

    auto setupButton = [&](sf::RectangleShape& button, sf::Text& text, const std::string& label, float yPos)
    {
        const sf::Vector2f btnSize(300.f, 60.f);
        button.setSize(btnSize);
        button.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
        button.setPosition({centerX, yPos});
        button.setFillColor(sf::Color(70, 70, 70));
        button.setOutlineThickness(2.f);
        button.setOutlineColor(sf::Color(100, 100, 100));

        text.setFont(font);
        text.setCharacterSize(24);
        text.setString(label);
        const sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.position.x + bounds.size.x/ 2.f, bounds.position.y + bounds.size.y/ 2.f});
        text.setPosition({centerX, yPos});
    };

    setupButton(ui.sandboxButton, ui.sandboxText, "Sandbox mode", buttonStartY);
    setupButton(ui.triviaButton, ui.triviaText, "Trivia mode", buttonStartY + buttonGap);
    setupButton(ui.leaderboardButton, ui.leaderboardText, "Leaderboard", buttonStartY + buttonGap * 2);
    setupButton(ui.exitButton, ui.exitText, "Exit", buttonStartY + buttonGap * 3);

    const sf::Vector2f tutSize(100.f, 40.f);
    ui.tutorialButton.setSize(tutSize);
    ui.tutorialButton.setOrigin({tutSize.x / 2.f, tutSize.y / 2.f});
    ui.tutorialButton.setPosition({rez.width - 80.f, 50.f});
    ui.tutorialButton.setFillColor(sf::Color(70, 70, 70));
    ui.tutorialButton.setOutlineThickness(2.f);
    ui.tutorialButton.setOutlineColor(sf::Color(100, 100, 100));

    ui.tutorialText.setFont(font);
    ui.tutorialText.setCharacterSize(20);
    ui.tutorialText.setString("Tutorial");
    const sf::FloatRect tutB = ui.tutorialText.getLocalBounds();
    ui.tutorialText.setOrigin({tutB.position.x + tutB.size.x/2.f, tutB.position.y + tutB.size.y/2.f});
    ui.tutorialText.setPosition(ui.tutorialButton.getPosition());

    ui.helpButton.setRadius(25.f);
    ui.helpButton.setOrigin({25.f, 25.f});
    ui.helpButton.setPosition({50.f, 50.f});
    ui.helpButton.setFillColor(sf::Color(0, 120, 200));
    ui.helpButton.setOutlineThickness(2.f);
    ui.helpButton.setOutlineColor(sf::Color::White);

    ui.helpButtonText.setFont(font);
    ui.helpButtonText.setCharacterSize(35);
    ui.helpButtonText.setString("?");
    ui.helpButtonText.setStyle(sf::Text::Bold);
    const sf::FloatRect hb = ui.helpButtonText.getLocalBounds();
    ui.helpButtonText.setOrigin({hb.size.x/2.f, hb.size.y/2.f + 8.f});
    ui.helpButtonText.setPosition(ui.helpButton.getPosition());

    ui.overlayBg.setSize({static_cast<float>(rez.width), static_cast<float>(rez.height)});
    ui.overlayBg.setFillColor(sf::Color(0, 0, 0, 220));

    float panelWidth = (rez.width - 80.f) / 2.f;
    float panelHeight = rez.height - 100.f;
    float leftPanelX = rez.width * 0.25f;
    float rightPanelX = rez.width * 0.75f;
    const int headerSize = (rez.height < 700) ? 24 : 28;
    const int bodySize = (rez.height < 700) ? 14 : 18;

    ui.rulesPanel.setSize({panelWidth, panelHeight});
    ui.rulesPanel.setOrigin({panelWidth / 2.f, panelHeight / 2.f});
    ui.rulesPanel.setPosition({leftPanelX, centerY});
    ui.rulesPanel.setFillColor(sf::Color(40, 44, 52));
    ui.rulesPanel.setOutlineThickness(3.f);
    ui.rulesPanel.setOutlineColor(sf::Color(0, 180, 230));

    ui.rulesTitle.setFont(font);
    ui.rulesTitle.setCharacterSize(headerSize);
    ui.rulesTitle.setString("Comenzile simulatorului");
    ui.rulesTitle.setStyle(sf::Text::Bold);
    ui.rulesTitle.setFillColor(sf::Color(0, 180, 230));
    const sf::FloatRect rtb = ui.rulesTitle.getLocalBounds();
    ui.rulesTitle.setOrigin({rtb.position.x + rtb.size.x/2.f, rtb.position.y + rtb.size.y/2.f});
    ui.rulesTitle.setPosition({leftPanelX, centerY - (panelHeight / 2.f) + 40.f});

    std::string rulesString =
        " Controale uzuale: \n"
        " MOUSE\n"
        "   L-Click + Drag : Mutare atomi\n"
        "   L-Click (in paleta): Selectare tip\n"
        "   L-Click : Creare atom\n"
        "   R-Click : Selectare / Legatura\n\n"
        " TASTATURA\n"
        "   DELETE : Stergere atom selectat\n"
        "   R : Resetat spatiul de lucru\n"
        "   ESC : Revenire la meniu\n\n"
        " INTERFATA\n"
        "   BUTON PLAY : Toggle mod fizic\n"
        "   R-CLICK (legatura C=C):\n"
        "   Toggle cis/trans";

    ui.rulesBody.setFont(font);
    ui.rulesBody.setCharacterSize(bodySize);
    ui.rulesBody.setString(rulesString);
    ui.rulesBody.setLineSpacing(1.4f);
    ui.rulesBody.setFillColor(sf::Color::White);
    const sf::FloatRect rbb = ui.rulesBody.getLocalBounds();
    ui.rulesBody.setOrigin({rbb.position.x + rbb.size.x/2.f, rbb.position.y + rbb.size.y/2.f});
    ui.rulesBody.setPosition({leftPanelX, centerY + 20.f});

    ui.purposePanel.setSize({panelWidth, panelHeight});
    ui.purposePanel.setOrigin({panelWidth / 2.f, panelHeight / 2.f});
    ui.purposePanel.setPosition({rightPanelX, centerY});
    ui.purposePanel.setFillColor(sf::Color(40, 44, 52));
    ui.purposePanel.setOutlineThickness(3.f);
    ui.purposePanel.setOutlineColor(sf::Color(255, 215, 0));

    ui.purposeTitle.setFont(font);
    ui.purposeTitle.setCharacterSize(headerSize);
    ui.purposeTitle.setString("Scopul simulatorului");
    ui.purposeTitle.setStyle(sf::Text::Bold);
    ui.purposeTitle.setFillColor(sf::Color(255, 215, 0));
    sf::FloatRect ptb = ui.purposeTitle.getLocalBounds();
    ui.purposeTitle.setOrigin({ptb.position.x + ptb.size.x/2.f, ptb.position.y + ptb.size.y/2.f});
    ui.purposeTitle.setPosition({rightPanelX, centerY - (panelHeight / 2.f) + 40.f});

    const std::string purposeString =
        "Acest simulator este o aplicatie educationala\n"
        "conceputa pentru a ajuta elevii sa inteleaga\n"
        "mai bine chimia\n\n"
        "Scop:\n"
        "- Legarea atomilor (prin electronii disponibili)\n"
        "- Vizualizare legaturi simple/duble/triple\n"
        "- Descoperirea de formule si molecule noi.\n\n"
        "Exploreaza modul 'Sandbox' pentru a invata\n"
        "codurile de culoare pentru fiecare atom si\n"
        "cum se conecteaza acestia.\n"
        "Cand esti pregatit, acceseaza modul 'Trivia'\n"
        "pentru a-ti testa cunostintele.\n\n";

    ui.purposeBody.setFont(font);
    ui.purposeBody.setCharacterSize(bodySize);
    ui.purposeBody.setString(purposeString);
    ui.purposeBody.setLineSpacing(1.4f);
    ui.purposeBody.setFillColor(sf::Color::White);
    const sf::FloatRect pbb = ui.purposeBody.getLocalBounds();
    ui.purposeBody.setOrigin({pbb.position.x + pbb.size.x/2.f, pbb.position.y + pbb.size.y/2.f});
    ui.purposeBody.setPosition({rightPanelX, centerY + 20.f});

    ui.closeInfo.setFont(font);
    ui.closeInfo.setCharacterSize(bodySize);
    ui.closeInfo.setString("[ Apasa oriunde pentru a inchide ]");
    ui.closeInfo.setFillColor(sf::Color(150, 150, 150));
    const sf::FloatRect cb = ui.closeInfo.getLocalBounds();
    ui.closeInfo.setOrigin({cb.position.x + cb.size.x/2.f, cb.position.y + cb.size.y/2.f});
    ui.closeInfo.setPosition({centerX, rez.height - 30.f});
}

void simulator_manager::UIMenuDraw(sf::RenderWindow& window, UIMenu& UI, const sf::Sprite& background, const bool showHelp)
{
    const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (!showHelp)
            {
                auto updateHover = [&](sf::RectangleShape& button)
                {
                    if (button.getGlobalBounds().contains(mousePos))
                        button.setFillColor(sf::Color(100, 100, 100));
                    else
                        button.setFillColor(sf::Color(70, 70, 70));
                };
                updateHover(UI.sandboxButton);
                updateHover(UI.triviaButton);
                updateHover(UI.leaderboardButton);
                updateHover(UI.exitButton);
                updateHover(UI.tutorialButton);

                if (UI.helpButton.getGlobalBounds().contains(mousePos))
                    UI.helpButton.setFillColor(sf::Color(0, 180, 230));
                else
                    UI.helpButton.setFillColor(sf::Color(0, 150, 200));
            }

            window.clear(sf::Color(100, 100, 100));

            window.draw(background);
            window.draw(UI.titleText);

            window.draw(UI.sandboxButton);
            window.draw(UI.sandboxText);

            window.draw(UI.triviaButton);
            window.draw(UI.triviaText);

            window.draw(UI.leaderboardButton);
            window.draw(UI.leaderboardText);

            window.draw(UI.exitButton);
            window.draw(UI.exitText);

            window.draw(UI.tutorialButton);
            window.draw(UI.tutorialText);

            window.draw(UI.helpButton);
            window.draw(UI.helpButtonText);
            if (showHelp)
            {
                window.draw(UI.overlayBg);
                window.draw(UI.rulesPanel);
                window.draw(UI.rulesTitle);
                window.draw(UI.rulesBody);
                window.draw(UI.purposePanel);
                window.draw(UI.purposeTitle);
                window.draw(UI.purposeBody);
                window.draw(UI.closeInfo);
            }

            window.display();
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

void simulator_manager::handleLeftClick(const sf::Vector2f mousePos, molecule& thisMolecule, const std::vector<std::shared_ptr<atom>>& atomPalette,
                                        int& selectedTemplateIndex, int& draggedAtomIndex, bool& isDragging, sf::Vector2f& dragOffset) const {
    if (mousePos.x < 200)
    {
        for (size_t i = 0; i < atomPalette.size(); ++i)
        {
            if (atomPalette[i]->getBounds().contains(mousePos))
            {
                selectedTemplateIndex = (int)i;
                notifyEvent("selectie");
            }
        }
    }
    else
    {
        int clickIndex = thisMolecule.findAtomAtPosition(mousePos);

        if (clickIndex != -1)
        {
            notifyEvent("selectie");
            isDragging = true;
            draggedAtomIndex = clickIndex;
            const auto atomPtr = thisMolecule.getAtom(draggedAtomIndex);
            if (atomPtr)
            {
                atomPtr->setAtomThickness(5.f);
                dragOffset = atomPtr->getAtomPosition() - mousePos;
            }
        }
        else
        {
            notifyEvent("selectie");
            const auto newEntity = atomPalette[selectedTemplateIndex]->clone();

            const auto atomPtr = std::static_pointer_cast<atom>(newEntity);
            atomPtr->setShowElectrons(true);

            if (const auto newIon = std::dynamic_pointer_cast<ion>(newEntity))
                thisMolecule.addAtom(newIon, mousePos);
            else {
                const auto newAtom = std::static_pointer_cast<atom>(newEntity);
                thisMolecule.addAtom(newAtom, mousePos);
            }
        }
    }
}

void simulator_manager::handleRightClick(const sf::Vector2f mousePos, molecule& thisMolecule,int& selectedAtomIndex) const {
    const int clickAtomIndex = thisMolecule.findAtomAtPosition(mousePos);

    if (clickAtomIndex != -1)
    {
        if (selectedAtomIndex == -1)
        {
            notifyEvent("selectie");
            selectedAtomIndex = clickAtomIndex;
            if (const auto atom = thisMolecule.getAtom(selectedAtomIndex))
                atom->setAtomThickness(5.f);
        }
        else
        {
            const int bondIndex = thisMolecule.findBondPosition(selectedAtomIndex, clickAtomIndex);
            const int v1 = thisMolecule.checkValenceLaws(clickAtomIndex);
            const int v2 = thisMolecule.checkValenceLaws(selectedAtomIndex);

            try
            {
                if (v1 && v2 && bondIndex == -1)
                {
                    notifyEvent("selectie");
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
                notifyEvent("stergere");
            }

            if (const auto atom = thisMolecule.getAtom(selectedAtomIndex))
                atom->setAtomThickness(2.f);

            selectedAtomIndex = -1;
        }
        return;
    }
    const int clickBondIndex = thisMolecule.findBondAtPosition(mousePos);
    if (clickBondIndex != -1)
    {
        const auto bondPtr = thisMolecule.getBond(clickBondIndex);
        if (const auto db = std::dynamic_pointer_cast<double_bond>(bondPtr))
        {
            if (thisMolecule.checkHydrocarbon())
            {
                db->toggleConfiguration();
                notifyEvent("selectie");
            }
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

void simulator_manager::scoreSaveMode(sf::RenderWindow& window, const int score, const sf::Font& font,const molecule& thisMolecule, leaderboard& board)
{
    if (score > 0)
    {
        const float winW = static_cast<float>(window.getSize().x);
        const float winH = static_cast<float>(window.getSize().y);

        const float centerX = winW / 2.f;
        const float centerY = winH / 2.f;

        sf::RectangleShape overlay({winW, winH});
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

                    const sf::FloatRect nb = nameText.getLocalBounds();
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

void simulator_manager::addObserver(const std::shared_ptr<event_observer>& obs)
{
    observers.push_back(obs);
}

resolution simulator_manager::newResolution()
{
    sf::RenderWindow launcher(sf::VideoMode({400, 300}), "Select Resolution", sf::Style::Titlebar | sf::Style::Close);
    sf::Font font;
    if (!font.openFromFile("Roboto-VariableFont_wdth,wght.ttf"))
        throw resourceMissingException("Roboto-VariableFont_wdth,wght.ttf");

    struct ResOption { unsigned int w, h; std::string label; };
    std::vector<ResOption> options = {
        {1000, 600, "1000 x 600 (Default)"},
        {1280, 720, "1280 x 720 (HD)"},
        //{1920, 1080, "1920 x 1080 (UHD)"},
        //{2048, 1080, "2048 x 1080 (2K)"}
    };

    std::vector<sf::Text> buttons;
    float startY = 60.f;

    sf::Text title(font, "Alege rezolutia:", 20);
    title.setPosition({20.f, 20.f});

    for (const auto& opt : options) {
        sf::Text t(font, opt.label, 20);
        t.setPosition({50.f, startY});
        buttons.push_back(t);
        startY += 50.f;
    }

    while (launcher.isOpen())
    {
        while (const std::optional event = launcher.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                launcher.close();
                std::exit(0);
            }

            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePress->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos = launcher.mapPixelToCoords(sf::Mouse::getPosition(launcher));

                    for (size_t i = 0; i < buttons.size(); ++i) {
                        if (buttons[i].getGlobalBounds().contains(mousePos)) {
                            launcher.close();
                            return {options[i].w, options[i].h};
                        }
                    }
                }
            }
        }

        // Hover Effect
        sf::Vector2f mousePos = launcher.mapPixelToCoords(sf::Mouse::getPosition(launcher));
        for (auto& btn : buttons) {
            if (btn.getGlobalBounds().contains(mousePos))
                btn.setFillColor(sf::Color::Yellow);
            else
                btn.setFillColor(sf::Color::White);
        }

        launcher.clear(sf::Color(60, 60, 60));
        launcher.draw(title);
        for (const auto& btn : buttons) launcher.draw(btn);
        launcher.display();
    }

    return {1000, 600};
}


simulator_manager &simulator_manager::getInstance()
{
    return instance;
}

simulator_manager simulator_manager::instance;
