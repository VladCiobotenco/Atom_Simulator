#include <fstream>
#include <vector>
#include <sstream>
#include "atom.hpp"

std::vector<atom> readAtoms(std::string fileName)
{
    std::vector<atom>inputAtoms;
    std::ifstream in(fileName, std::ios::in);
    std::string line;
    while (std::getline(in, line))
    {
        std::stringstream ss(line);
        std::string atomName, symbol;
        int period,group,atomicNumber,atomicMass;
        ss>>period>>group>>atomicNumber>>atomicMass>>atomName>>symbol;
        atom temporaryAtom(period,group,atomicNumber,atomicMass,atomName,symbol);
        inputAtoms.push_back(temporaryAtom);
    }
    return inputAtoms;
}