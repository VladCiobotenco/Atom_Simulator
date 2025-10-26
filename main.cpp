#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <optional>
#include <chrono>

#include "sources/atom.hpp"
#include "sources/bond.hpp"
#include "sources/molecule.hpp"

int main()
{
    std::ifstream in("input/elements.txt");

    atom H(1,1,1,1,"Hidrogen","H");
    atom C(2,4,6,12,"Carbon","C");
    atom N(2,5,7,14,"Azot","N");
    atom O(2,6,8,16,"Oxigen","O");
    std::cout<<C<<" "<<C.atomValence()<<"\n";

    molecule H2O("Water");
    H2O.addAtom(H);
    H2O.addAtom(H);
    H2O.addAtom(O);
    std::cout<<H2O<<" "<<H2O.moleculeMass()<<"\n";

    bond A(0,1,"simple");
    bond B(1,2,"simple");
    H2O.addBond(A);
    H2O.addBond(B);

    H2O.removeBond();
    H2O.removeBond();

    ///Work in progress
    sf::RenderWindow mainScreen(sf::VideoMode({800, 600}), "Atom Simulator");

    int draggedAtomIndex = -1;
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
                            atom& clickedAtom=H2O.getAtom(draggedAtomIndex);
                            dragOffset=clickedAtom.getAtomPosition()-mousePosition;
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