#include <string>
#include <SFML/Graphics.hpp>
#include <optional>

#include "../sources/atom.hpp"
#include "../sources/bond.hpp"
#include "../sources/molecule.hpp"
#include "../sources/ion.hpp"

#include "simulator_manager.hpp"
#include "audio_manager.hpp"

#include <iostream>

#include "exceptions.hpp"

void simulator_manager::simulationStart(const std::string& windowName, molecule& thisMolecule, const sf::Font& font, const std::vector<atom>& templateAtoms, const std::vector<ion>& templateIons, chemical_database& database, audio_manager& audio)
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

    ///Work in progress
    sf::RenderWindow mainScreen(sf::VideoMode({1000, 600}), windowName);
    mainScreen.setFramerateLimit(60);

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

    while (mainScreen.isOpen())
    {
        sf::FloatRect templateBounds = atomPalette[selectedTemplateIndex]->getBounds();
        selectionBox.setSize({templateBounds.size.x + 10.f, templateBounds.size.y + 10.f});
        selectionBox.setOrigin({5.f, 5.f});
        selectionBox.setPosition(atomPalette[selectedTemplateIndex]->getAtomPosition() - sf::Vector2f(templateBounds.size.x/2, templateBounds.size.y/2));

        while (const std::optional event = mainScreen.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                mainScreen.close();

            if (event->is<sf::Event::KeyPressed>())
            {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if (keyPressed->code==sf::Keyboard::Key::Escape)
                    mainScreen.close();

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
                    sf::Vector2f mousePosition = mainScreen.mapPixelToCoords(pixelPosition);
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
                    sf::Vector2f mousePosition = mainScreen.mapPixelToCoords(pixelPosition);
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
            sf::Vector2i pixelPos = sf::Mouse::getPosition(mainScreen);
            sf::Vector2f worldPos = mainScreen.mapPixelToCoords(pixelPos);
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
            sf::Vector2i pixelPos = sf::Mouse::getPosition(mainScreen);
            sf::Vector2f worldPos = mainScreen.mapPixelToCoords(pixelPos);
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


        mainScreen.clear(sf::Color(232, 219, 135));
        thisMolecule.draw(mainScreen);
        mainScreen.draw(atomMenuBackground);
        mainScreen.draw(dashboardBox);
        mainScreen.draw(dashboardText);
        for (const auto& menuAtom : atomPalette) {
            menuAtom->draw(mainScreen);
        }
        mainScreen.draw(selectionBox);
        if (infoBoxVisibility)
        {
            mainScreen.draw(infoBox);
            mainScreen.draw(infoText);
        }
        mainScreen.draw(formulaBox);
        mainScreen.draw(formulaText);

        mainScreen.display();
    }
}
