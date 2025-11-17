#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <optional>
#include <chrono>

#include "../sources/atom.hpp"
#include "../sources/bond.hpp"
#include "../sources/molecule.hpp"
#include "../sources/input.hpp"

#include "simulator_manager.hpp"

void simulator_manager::simulationStart(std::string& windowName, molecule testMolecule)
{
    sf::Font font("../fonts/Roboto-VariableFont_wdth,wght.ttf");
    sf::Text infoText(font);
    infoText.setCharacterSize(14);
    infoText.setFillColor(sf::Color::Black);

    sf::RectangleShape infoBox;
    infoBox.setFillColor(sf::Color::White);
    infoBox.setOutlineColor(sf::Color::Black);
    infoBox.setOutlineThickness(1.f);
    infoBox.setSize({150.f, 25.f});
    bool infoBoxVisibility = false;


    ///Work in progress
    sf::RenderWindow mainScreen(sf::VideoMode({800, 600}), windowName);

    int draggedAtomIndex = -1; //folosit pentru dragging
    int selectedAtomIndex = -1; //folosit pentru bonding
    bool isDragging = false;
    sf::Vector2f dragOffset;
    while (mainScreen.isOpen())
    {
        while (const std::optional event = mainScreen.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                mainScreen.close();

            if (event->is<sf::Event::KeyPressed>())
            {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if (keyPressed->code==sf::Keyboard::Key::Escape)
                    mainScreen.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
               const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i pixelPosition = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                    sf::Vector2f mousePosition = mainScreen.mapPixelToCoords(pixelPosition);
                    int clickAtomIndex=testMolecule.findAtomAtPosition(mousePosition);
                    if (clickAtomIndex != -1)
                    {
                        isDragging=true;
                        draggedAtomIndex=clickAtomIndex;
                        atom* clickedAtom=testMolecule.getAtom(draggedAtomIndex);
                        clickedAtom->setAtomThickness(5.f);
                        dragOffset=clickedAtom->getAtomPosition()-mousePosition;
                    }
                }
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    sf::Vector2i pixelPosition = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                    sf::Vector2f mousePosition = mainScreen.mapPixelToCoords(pixelPosition);
                    int clickAtomIndex=testMolecule.findAtomAtPosition(mousePosition);
                    if (clickAtomIndex != -1)
                    {
                        if (selectedAtomIndex == -1)
                        {
                            selectedAtomIndex=clickAtomIndex;
                            atom* selectedAtom=testMolecule.getAtom(selectedAtomIndex);
                            selectedAtom->setAtomThickness(5.f);
                        }

                        else
                        {
                            int bondIndex=testMolecule.findBondPosition(selectedAtomIndex, clickAtomIndex);
                            if (testMolecule.checkValenceLaws(clickAtomIndex) && testMolecule.checkValenceLaws(selectedAtomIndex) && bondIndex==-1)
                                testMolecule.addBond(selectedAtomIndex,clickAtomIndex,"simple");
                            else if (bondIndex!=-1)
                                testMolecule.removeBond(bondIndex);
                            atom* selectedAtom=testMolecule.getAtom(selectedAtomIndex);
                            selectedAtom->setAtomThickness(2.f);
                            selectedAtomIndex=-1;
                        }
                    }
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                const sf::Event::MouseButtonReleased* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>();
                if (mouseButtonReleased->button == sf::Mouse::Button::Left && isDragging == true)
                {
                    atom* draggedAtom = testMolecule.getAtom(draggedAtomIndex);
                    draggedAtom->setAtomThickness(2.f);
                    isDragging=false;
                    draggedAtomIndex=-1;
                }

            }
        }

        if (isDragging && draggedAtomIndex != -1)
        {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(mainScreen);
            sf::Vector2f worldPos = mainScreen.mapPixelToCoords(pixelPos);
            atom* currentAtom = testMolecule.getAtom(draggedAtomIndex);
            currentAtom->move(worldPos+dragOffset);
            currentAtom->restrictAtomToWindow(mainScreen);
            testMolecule.updateBondsPositions();
            infoBoxVisibility=false;
        }
        if (!isDragging)
        {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(mainScreen);
            sf::Vector2f worldPos = mainScreen.mapPixelToCoords(pixelPos);
            int hoveredAtomIndex=testMolecule.findAtomAtPosition(worldPos);
            if (hoveredAtomIndex != -1)
            {
                infoBoxVisibility=true;
                const atom* hoveredAtom = testMolecule.getAtom(hoveredAtomIndex);
                std::string info = hoveredAtom->getName() + " (" + hoveredAtom->getSymbol() + ")";
                infoText.setString(info);

                sf::Vector2f infoBoxPosition=worldPos+sf::Vector2f(10.f,10.f);
                infoBox.setPosition(infoBoxPosition);
                infoText.setPosition(infoBoxPosition+sf::Vector2f(40.f,0.f));
            }
            else infoBoxVisibility=false;
        }

        mainScreen.clear(sf::Color::Cyan);
        testMolecule.draw(mainScreen);
        if (infoBoxVisibility)
        {
            mainScreen.draw(infoBox);
            mainScreen.draw(infoText);
        }
        mainScreen.display();
    }
}
