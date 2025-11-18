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
#include "sources/simulator_manager.hpp"


int main()
{
    const sf::Font font("../fonts/Roboto-VariableFont_wdth,wght.ttf");

    std::vector<atom> inputAtoms = readAtomsFromJson("atoms.json");
    if (inputAtoms.empty()) {
        std::cerr << "Nu s-a citit niciun atom\n";
        return 1;
    }

    std::vector<ion> inputIons = readIonsFromJson("ions.json", font);
    if (inputIons.empty()) {
        std::cerr << "Nu s-a citit niciun ion\n";
        return 1;
    }

    //std::cout<<inputIons[0];

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

    std::cout<<testMolecule<<" "<<testMolecule.moleculeMass()<<"\n";

    std::string gameWindowName="Atom Simulator";

    simulator_manager::simulationStart(gameWindowName, testMolecule, font);

    testMolecule.removeAtom();
    testMolecule.removeAtom();
    testMolecule.removeAtom();
    testMolecule.removeAtom();

    return 0;
}
