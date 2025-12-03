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
        const sf::Font font("../fonts/Roboto-VariableFont_wdth,wght.ttf");


        std::vector<atom> inputAtoms = readAtomsFromJson("atoms.json");

        std::vector<ion> inputIons = readIonsFromJson("ions.json", font);
        ion Na = inputIons[0];
        ion Cl = inputIons[1];
        std::cout<<Na;

        atom H = inputAtoms[0];
        atom C = inputAtoms[1];
        atom N = inputAtoms[2];
        atom O = inputAtoms[3];

        std::cout<<C<<" "<<C.atomValence()<<"\n";

        molecule testMolecule("practiceMolecule");

        testMolecule.addAtom(C);
        testMolecule.addAtom(O);
        testMolecule.addAtom(N);
        testMolecule.addAtom(H);
        testMolecule.addAtom(Na);
        testMolecule.addAtom(Cl);

        std::cout<<testMolecule<<" "<<testMolecule.moleculeMass()<<"\n";

        std::string gameWindowName="Atom Simulator";

        simulator_manager::simulationStart(gameWindowName, testMolecule, font);

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

    catch (const atomSimulatorExceptions& e)
    {
        std::cerr << "EROARE: " << e.what() << "\n";
    }

    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }


    return 0;
}
