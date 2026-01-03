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

void simulator_manager::simulation()
{
    sf::Font font;
    if (!font.openFromFile("assets/Roboto-VariableFont_wdth,wght.ttf"))
        throw resourceMissingException("assets/Roboto-VariableFont_wdth,wght.ttf");

    std::vector<atom> inputAtoms = readAtomsFromJson("atoms.json");
    std::vector<ion> inputIons = readIonsFromJson("ions.json", font);

    chemical_database database;
    database.loadIntoDatabase("elements.json");

    audio_manager audio;
    audio.playMusic("assets/MainMusic.ogg");
    audio.loadSound("selectie","assets/click-selectare.wav");
    audio.loadSound("stergere","assets/click-stergere.wav");

    sf::RenderWindow window(sf::VideoMode({1000, 600}), "Atom Simulator");

    sf::Text titleText(font);
        titleText.setCharacterSize(50);
        titleText.setString("Atom Simulator");
        titleText.setFillColor(sf::Color::White);
        titleText.setStyle(sf::Text::Bold);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin({titleBounds.size.x / 2, titleBounds.size.y / 2});
        titleText.setPosition({400.f, 100.f});

        sf::Vector2f buttonSize(300.f, 60.f);
        float buttonX = 400.f;
        float startY = 250.f;
        float gapY = 100.f;

        sf::RectangleShape sandboxButton(buttonSize);
        sandboxButton.setOrigin({buttonSize.x / 2, buttonSize.y / 2});
        sandboxButton.setPosition({buttonX, startY});
        sandboxButton.setFillColor(sf::Color(70, 70, 70));
        sf::Text sandboxText(font);
        sandboxText.setCharacterSize(24);
        sandboxText.setString("Sandbox mode");
        sf::FloatRect sandboxBounds = sandboxText.getLocalBounds();
        sandboxText.setOrigin({sandboxBounds.position.x / 2, sandboxBounds.position.y / 2});
        sandboxText.setPosition({buttonX, startY - 5.f});

        sf::RectangleShape triviaButton(buttonSize);
        triviaButton.setOrigin({buttonSize.x / 2, buttonSize.y / 2});
        triviaButton.setPosition({buttonX, startY + gapY});
        triviaButton.setFillColor(sf::Color(70, 70, 70));
        sf::Text triviaText(font);
        triviaText.setCharacterSize(24);
        triviaText.setString("Trivia mode");
        sf::FloatRect triviaBounds = triviaText.getLocalBounds();
        triviaText.setOrigin({triviaBounds.position.x / 2, triviaBounds.position.y / 2});
        triviaText.setPosition({buttonX, startY + gapY - 5.f});

        sf::RectangleShape exitBtn(buttonSize);
        exitBtn.setOrigin({buttonSize.x / 2, buttonSize.y / 2});
        exitBtn.setPosition({buttonX, startY + gapY * 2});
        exitBtn.setFillColor(sf::Color(70, 70, 70));
        sf::Text exitText(font);
        exitText.setCharacterSize(24);
        exitText.setString("Exit");
        sf::FloatRect exitBounds = exitText.getLocalBounds();
        exitText.setOrigin({exitBounds.position.x / 2, exitBounds.position.y / 2});
        exitText.setPosition({buttonX, startY + gapY * 2 - 5.f});

        while (window.isOpen()) {
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
                        if (sandboxButton.getGlobalBounds().contains(mousePos)) {
                            audio.playSound("selectie");
                            molecule testMolecule("sandboxMolecule");
                            sandboxMode(window, testMolecule, font, inputAtoms, inputIons, database, audio);
                            testMolecule.removeEntities();
                        }
                        else if (triviaButton.getGlobalBounds().contains(mousePos)) {
                            audio.playSound("selectie");
                            triviaMode();
                        }
                        else if (exitBtn.getGlobalBounds().contains(mousePos)) {
                            window.close();
                            return;
                        }
                    }
                }

                if (!window.isOpen())
                    break;

                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                auto updateHover = [&](sf::RectangleShape& btn) {
                    if (btn.getGlobalBounds().contains(mousePos))
                        btn.setFillColor(sf::Color(100, 100, 100));
                    else
                        btn.setFillColor(sf::Color(70, 70, 70));
                };
                updateHover(sandboxButton);
                updateHover(triviaButton);
                updateHover(exitBtn);

                window.clear(sf::Color(30, 30, 30));
                window.draw(titleText);

                window.draw(sandboxButton);
                window.draw(sandboxText);

                window.draw(triviaButton);
                window.draw(triviaText);

                window.draw(exitBtn);
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
                        thisMolecule.removeEntity(selectedAtomIndex);
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
                                thisMolecule.removeEntity(bondIndex);
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

void simulator_manager::triviaMode()
{
    return;
}

simulator_manager &simulator_manager::getInstance()
{
    return instance;
}
