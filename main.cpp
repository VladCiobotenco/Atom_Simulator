#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>


#include "sources/atom.hpp"
#include "sources/molecule.hpp"
#include "sources/input.hpp"
#include "sources/simulator_manager.hpp"

#include "sources/exceptions.hpp"


int main()
{
    try
    {
        sf::Font font;
        if (!font.openFromFile("../fonts/Roboto-VariableFont_wdth,wght.ttf"))
            throw resourceMissingException("../fonts/Roboto-VariableFont_wdth,wght.ttf");


        std::vector<atom> inputAtoms = readAtomsFromJson("atoms.json");
        std::vector<ion> inputIons = readIonsFromJson("ions.json", font);

        molecule testMolecule("practiceMolecule");
        std::string gameWindowName="Atom Simulator";

        simulator_manager::simulationStart(gameWindowName, testMolecule, font, inputAtoms, inputIons);

        testMolecule.removeEntities();
    }

    catch (const fileLoadingException& e)
    {
        std::cerr << "EROARE: " << e.what() << "\n";
        std::cerr << "Verificati daca fisierul specificat se afla la locatia corecta!\n";
        return 1;
    }

    catch (const fileFormatException& e)
    {
        std::cerr << "EROARE: " << e.what() << "\n";
        std::cerr << "Verificati daca fisierul specificat contine informatii corecte!\n";
        return 1;
    }

    catch (const resourceMissingException& e)
    {
        std::cerr << "EROARE: " << e.what() << "\n";
        std::cerr << "Verificati daca resursa specificata exista!\n";
        return 1;
    }

    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }


    return 0;
}
