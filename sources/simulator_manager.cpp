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

#include "../include/exceptions.hpp"
#include "../include/leaderboard.hpp"

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

            window.display();
        }
    }
}

void simulator_manager::sandboxMode(sf::RenderWindow& window, molecule& thisMolecule, const sf::Font& font, const std::vector<atom>& templateAtoms, const std::vector<ion>& templateIons, chemical_database& database, audio_manager& audio)
{
    /// Crearea unei palete de atomi in zona de menu
    std::vector<std::shared_ptr<atom>> atomPalette;

    float currentX = 100.f;
    float currentY = 50.f;
    for (const auto& templateData : templateAtoms)
    {
        auto menuButton = std::make_shared<atom>(templateData);
        menuButton->setPosition({currentX, currentY});
        atomPalette.push_back(std::move(menuButton));
        currentY += 75.f;
    }

    for (const auto& templateData : templateIons) {
        auto menuButton = std::make_shared<ion>(templateData);
        menuButton->setPosition({currentX, currentY});
        atomPalette.push_back(std::move(menuButton));
        currentY += 75.f;
    }

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
                    }

                if (keyPressed->code==sf::Keyboard::Key::R)
                    thisMolecule.removeEntities();
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i pixelPosition = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                    sf::Vector2f mousePosition = window.mapPixelToCoords(pixelPosition);
                    if (mousePosition.x < 200)
                    {
                        for (size_t i = 0; i < atomPalette.size(); ++i)
                        {
                            if (atomPalette[i]->getBounds().contains(mousePosition))
                            {
                                selectedTemplateIndex = (int)i;
                                std::cout << "Selected: " << atomPalette[i]->getName() << "\n";
                            }
                        }
                    }
                    else
                    {
                        audio.playSound("selectie");
                        int clickAtomIndex=thisMolecule.findAtomAtPosition(mousePosition);
                        if (clickAtomIndex != -1)
                        {
                            isDragging=true;
                            draggedAtomIndex=clickAtomIndex;
                            const auto clickedAtom=thisMolecule.getAtom(draggedAtomIndex);
                            clickedAtom->setAtomThickness(5.f);
                            dragOffset=clickedAtom->getAtomPosition()-mousePosition;
                        }
                        else
                            /// Se creeaza un atom/ion
                        {
                            auto newEntity = atomPalette[selectedTemplateIndex]->clone();
                            auto newEntityAtom = std::dynamic_pointer_cast<atom>(newEntity);
                            newEntityAtom->setShowElectrons(true);
                            if (auto ionPtr = std::dynamic_pointer_cast<ion>(newEntity))
                                thisMolecule.addAtom(ionPtr,mousePosition);
                            else if (auto atomPtr = std::dynamic_pointer_cast<atom>(newEntity))
                                thisMolecule.addAtom(atomPtr, mousePosition);
                        }
                    }

                }
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    sf::Vector2i pixelPosition = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                    sf::Vector2f mousePosition = window.mapPixelToCoords(pixelPosition);
                    int clickAtomIndex=thisMolecule.findAtomAtPosition(mousePosition);
                    if (clickAtomIndex != -1)
                    {
                        if (selectedAtomIndex == -1)
                        {
                            audio.playSound("selectie");
                            selectedAtomIndex=clickAtomIndex;
                            const auto selectedAtom=thisMolecule.getAtom(selectedAtomIndex);
                            selectedAtom->setAtomThickness(5.f);
                        }

                        else
                        {
                            ///Se creaza o legatura
                            int bondIndex=thisMolecule.findBondPosition(selectedAtomIndex, clickAtomIndex);
                            int availableBonds1=thisMolecule.checkValenceLaws(clickAtomIndex);
                            int availableBonds2=thisMolecule.checkValenceLaws(selectedAtomIndex);
                            try
                            {
                                if (availableBonds1 && availableBonds2 && bondIndex==-1)
                                {
                                    audio.playSound("selectie");
                                    if (availableBonds1>=4 && availableBonds2>=4)
                                        thisMolecule.addBond(selectedAtomIndex,clickAtomIndex,"quad_bond");
                                    else if (availableBonds1>=3 && availableBonds2>=3)
                                        thisMolecule.addBond(selectedAtomIndex,clickAtomIndex,"triple_bond");
                                    else if (availableBonds1>=2 && availableBonds2>=2)
                                        thisMolecule.addBond(selectedAtomIndex,clickAtomIndex,"double_bond");
                                    else
                                        thisMolecule.addBond(selectedAtomIndex,clickAtomIndex,"single_bond");
                                    thisMolecule.updateBondsPositions();
                                }
                            }
                            catch (const atomSimulatorExceptions& e)
                            {
                                std::cerr << "EROARE: " << e.what() << "\n";
                            }

                            if (bondIndex!=-1)
                            {
                                thisMolecule.removeBond(bondIndex);
                                audio.playSound("stergere");
                            }

                            const auto selectedAtom=thisMolecule.getAtom(selectedAtomIndex);
                            selectedAtom->setAtomThickness(2.f);
                            selectedAtomIndex=-1;
                        }
                    }
                }
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

    std::vector<std::shared_ptr<atom>> atomPalette;
    float currentX = 100.f;
    float currentY = 50.f;

    for (const auto& templateData : templateAtoms)
    {
        auto menuButton = std::make_shared<atom>(templateData);
        menuButton->setPosition({currentX, currentY});
        atomPalette.push_back(std::move(menuButton));
        currentY += 75.f;
    }
    for (const auto& templateData : templateIons)
    {
        auto menuButton = std::make_shared<ion>(templateData);
        menuButton->setPosition({currentX, currentY});
        atomPalette.push_back(std::move(menuButton));
        currentY += 75.f;
    }

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
                    return;
                }
                if (keyEvent->code == sf::Keyboard::Key::Delete && selectedAtomIndex != -1)
                {
                    thisMolecule.removeAtom(selectedAtomIndex);
                    selectedAtomIndex = -1;
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
                        std::string userFormula = thisMolecule.getMolecularFormula();

                        if (waitNewMolecule == false && userFormula == targetFormula)
                        {
                            score++;
                            scoreText.setString("Scor: " + std::to_string(score));
                            feedbackText.setString("Corect! Urmatoarea molecula este...");
                            feedbackText.setFillColor(sf::Color::Green);
                            feedbackText.setPosition({400.f, 550.f});
                            audio.playSound("corect");
                            clock.restart();
                            waitNewMolecule = true;
                        }

                        else
                        {
                            feedbackText.setString("Gresit! Mai incearca!");
                            feedbackText.setFillColor(sf::Color::Red);
                            sf::FloatRect fb = feedbackText.getLocalBounds();
                            feedbackText.setPosition({500.f - fb.size.x/2, 550.f});
                            audio.playSound("gresit");
                        }
                    }

                    else if (mousePos.x < 200)
                    {
                        for (size_t i = 0; i < atomPalette.size(); ++i)
                        {
                            if (atomPalette[i]->getBounds().contains(mousePos))
                            {
                                selectedTemplateIndex = static_cast<int>(i);
                                audio.playSound("selectie");
                            }
                        }
                    }

                    else
                    {
                        int clickIndex = thisMolecule.findAtomAtPosition(mousePos);

                        if (clickIndex != -1)
                        {
                            isDragging = true;
                            draggedAtomIndex = clickIndex;
                            auto atom = thisMolecule.getAtom(draggedAtomIndex);
                            if(atom)
                            {
                                atom->setAtomThickness(5.f);
                                dragOffset = atom->getAtomPosition() - mousePos;
                            }
                        }
                        else
                        {
                            auto newEntity = atomPalette[selectedTemplateIndex]->clone();
                            auto newAtom = std::dynamic_pointer_cast<atom>(newEntity);
                            if(newAtom)
                                newAtom->setShowElectrons(true);

                            if (auto ionPtr = std::dynamic_pointer_cast<ion>(newEntity))
                                thisMolecule.addAtom(ionPtr, mousePos);
                            else if (auto atomPtr = std::dynamic_pointer_cast<atom>(newEntity))
                                thisMolecule.addAtom(atomPtr, mousePos);
                        }
                    }
                }

                else if (mousePress->button == sf::Mouse::Button::Right)
                {
                    int clickAtomIndex = thisMolecule.findAtomAtPosition(mousePos);

                    if (clickAtomIndex != -1)
                    {
                        if (selectedAtomIndex == -1)
                        {
                            audio.playSound("selectie");
                            selectedAtomIndex = clickAtomIndex;
                            auto atom = thisMolecule.getAtom(selectedAtomIndex);
                            if(atom) atom->setAtomThickness(5.f);

                        }
                        else
                        {
                            ///Se creaza o legatura
                            int bondIndex=thisMolecule.findBondPosition(selectedAtomIndex, clickAtomIndex);
                            int availableBonds1=thisMolecule.checkValenceLaws(clickAtomIndex);
                            int availableBonds2=thisMolecule.checkValenceLaws(selectedAtomIndex);
                            try
                            {
                                if (availableBonds1 && availableBonds2 && bondIndex==-1)
                                {
                                    audio.playSound("selectie");
                                    if (availableBonds1>=4 && availableBonds2>=4)
                                        thisMolecule.addBond(selectedAtomIndex,clickAtomIndex,"quad_bond");
                                    else if (availableBonds1>=3 && availableBonds2>=3)
                                        thisMolecule.addBond(selectedAtomIndex,clickAtomIndex,"triple_bond");
                                    else if (availableBonds1>=2 && availableBonds2>=2)
                                        thisMolecule.addBond(selectedAtomIndex,clickAtomIndex,"double_bond");
                                    else
                                        thisMolecule.addBond(selectedAtomIndex,clickAtomIndex,"single_bond");
                                    thisMolecule.updateBondsPositions();
                                }
                            }
                            catch (const atomSimulatorExceptions& e)
                            {
                                std::cerr << "EROARE: " << e.what() << "\n";
                            }

                            if (bondIndex!=-1)
                            {
                                thisMolecule.removeBond(bondIndex);
                                audio.playSound("stergere");
                            }

                            const auto selectedAtom=thisMolecule.getAtom(selectedAtomIndex);
                            selectedAtom->setAtomThickness(2.f);
                            selectedAtomIndex=-1;
                        }
                    }
                }
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

        window.clear(sf::Color(40, 44, 52)); // Darker background for Trivia

        window.draw(targetText);
        window.draw(scoreText);
        window.draw(feedbackText);

        thisMolecule.draw(window);
        window.draw(atomMenuBackground);
        for (const auto& btn : atomPalette) btn->draw(window);
        window.draw(selectionBox);

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
        //window.draw(listText);
        window.draw(backText);
        window.draw(leaderboardPanel);
        for (const auto& line: scoreLines)
            window.draw(line);

        window.display();
    }
}

simulator_manager &simulator_manager::getInstance()
{
    return instance;
}

simulator_manager simulator_manager::instance;
