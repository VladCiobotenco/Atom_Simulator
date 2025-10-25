#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include <optional>

#include "sources/atom.hpp"
#include "sources/bond.hpp"
#include "sources/molecule.hpp"

int main()
{
    atom H(1,1,1,1,"Hidrogen","H");
    atom C(2,4,6,12,"Carbon","C");
    atom N(2,5,7,14,"Azot","N");
    atom O(2,6,8,16,"Oxigen","O");
    std::cout<<C;

    molecule H2O("Water");
    H2O.addAtom(H);
    H2O.addAtom(H);
    H2O.addAtom(O);
    std::cout<<H2O;

    H2O.removeAtom();
    H2O.removeAtom();
    H2O.removeAtom();

    ///Work in progress
    sf::RenderWindow mainScreen(sf::VideoMode({800, 600}), "Atom Simulator");
    while (mainScreen.isOpen())
    {
        while (const std::optional event = mainScreen.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                mainScreen.close();
        }
        mainScreen.clear(sf::Color::Cyan);
        N.draw(mainScreen);
        mainScreen.display();
    }


    return 0;
}