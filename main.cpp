#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include <optional>
#include <chrono>
#include <thread>

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

    H2O.removeAtom();
    H2O.removeAtom();
    H2O.removeAtom();

    ///Work in progress
    sf::RenderWindow mainScreen(sf::VideoMode({800, 600}), "Atom Simulator");

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
                if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2i pixelPosition = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                        sf::Vector2f mousePosition = mainScreen.mapPixelToCoords(pixelPosition);
                        if (H.getBounds().contains(mousePosition))
                        {
                            isDragging = true;
                            dragOffset = H.getAtomPosition() - mousePosition;
                        }
                    }
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                isDragging=false;

            }
        }

        if (isDragging)
        {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(mainScreen);
            sf::Vector2f worldPos = mainScreen.mapPixelToCoords(pixelPos);
            H.move(worldPos + dragOffset);
        }

        mainScreen.clear(sf::Color::Cyan);
        H.draw(mainScreen);
        mainScreen.display();
    }


    return 0;
}