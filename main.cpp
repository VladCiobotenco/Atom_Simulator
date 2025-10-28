#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <optional>
#include <chrono>

#include "sources/atom.hpp"
#include "sources/bond.hpp"
#include "sources/molecule.hpp"
#include "sources/input.hpp"


int main()
{
    std::vector<atom> inputAtoms = readAtomsFromJson("F:/Facultate/POO/Atom_Simulator/sources/elements.json");
    if (inputAtoms.empty()) {
        std::cerr << "Nu s-a citit niciun atom\n";
        return 1;
    }


    atom H = inputAtoms[0];
    atom C = inputAtoms[1];
    atom N = inputAtoms[2];
    atom O = inputAtoms[3];

    /*atom H(1,1,1,1,"Hidrogen","H");
    atom C(2,4,6,12,"Carbon","C");
    atom N(2,5,7,14,"Azot","N");
    atom O(2,6,8,16,"Oxigen","O");*/
    std::cout<<C<<" "<<C.atomValence()<<"\n";

    molecule H2O("Water");
    H2O.addAtom(O);
    H2O.addAtom(H);
    H2O.addAtom(H);

    std::cout<<H2O<<" "<<H2O.moleculeMass()<<"\n";

    ///Work in progress
    sf::RenderWindow mainScreen(sf::VideoMode({800, 600}), "Atom Simulator");

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

            if (event->is<sf::Event::MouseButtonPressed>())
            {
               const sf::Event::MouseButtonPressed* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i pixelPosition = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                    sf::Vector2f mousePosition = mainScreen.mapPixelToCoords(pixelPosition);
                    int clickAtomIndex=H2O.findAtomAtPosition(mousePosition);
                    if (clickAtomIndex != -1)
                    {
                        isDragging=true;
                        draggedAtomIndex=clickAtomIndex;
                        const atom& clickedAtom=H2O.getAtom(draggedAtomIndex);
                        dragOffset=clickedAtom.getAtomPosition()-mousePosition;
                    }
                }
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    sf::Vector2i pixelPosition = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                    sf::Vector2f mousePosition = mainScreen.mapPixelToCoords(pixelPosition);
                    int clickAtomIndex=H2O.findAtomAtPosition(mousePosition);
                    if (clickAtomIndex != -1)
                    {
                        if (selectedAtomIndex == -1)
                        {
                            selectedAtomIndex=clickAtomIndex;
                        }
                        else
                        {
                            H2O.addBond(selectedAtomIndex,clickAtomIndex,"simple");
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
                    isDragging=false;
                    draggedAtomIndex=-1;
                }

            }
        }

        if (isDragging && draggedAtomIndex != -1)
        {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(mainScreen);
            sf::Vector2f worldPos = mainScreen.mapPixelToCoords(pixelPos);
            atom& currentAtom = H2O.getAtom(draggedAtomIndex);
            currentAtom.move(worldPos+dragOffset);
            currentAtom.restrictAtomToWindow(mainScreen);
            H2O.updateBondsPositions();
        }

        mainScreen.clear(sf::Color::Cyan);
        H2O.draw(mainScreen);
        mainScreen.display();
    }


    H2O.removeAtom();
    H2O.removeAtom();
    H2O.removeAtom();

    return 0;
}