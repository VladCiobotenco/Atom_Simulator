#include "molecule.hpp"
#include <iostream>
#include <utility>
#include <stdexcept>

molecule::molecule(std::string  t):name(std::move(t))
{
    std::cout<<"O molecula a fost construita\n";
}
//molecule::molecule(const molecule& other): name(other.name), atomsList(other.atomsList), bondsList(other.bondsList){}
molecule::molecule(const molecule& other): name(other.name)
{
    atomsList.reserve(other.atomsList.size());
    for (const auto& atom : other.atomsList)
        atomsList.push_back(atom->clone());
    bondsList.reserve(other.bondsList.size());
    for (const auto& bond : other.bondsList)
        bondsList.push_back(bond->clone());
}
// molecule& molecule::operator=(const molecule& other)
// {
//     if (this != &other)
//     {
//         name = other.name;
//         atomsList = other.atomsList;
//         bondsList = other.bondsList;
//     }
//     return *this;
// }
molecule& molecule::operator=(const molecule& other)
{
    if (this != &other) {
        molecule temp(other);
        std::swap(name, temp.name);
        std::swap(atomsList, temp.bondsList);
        std::swap(bondsList, temp.bondsList);
    }
    return *this;
}
molecule::~molecule(){std::cout<<"O molecula a fost distrusa\n";}

// void molecule::addAtom(const atom& ATOM)
// {
//     atomsList.push_back(ATOM);
// }
void molecule::addAtom(const atom& tempAtom)
{
    std::unique_ptr<atom> newAtom = std::make_unique<atom>(tempAtom);
    atomsList.push_back(std::move(newAtom));
}
void molecule::removeAtom()
{
    atomsList.pop_back();
}
// void molecule::addBond(const int index1, const int index2, const std::string& type)
// {
//     const sf::Vector2f atom1Position = atomsList[index1].getAtomPosition();
//     const sf::Vector2f atom2Position = atomsList[index2].getAtomPosition();
//     const bond temporaryBond(type,index1,index2,atom1Position,atom2Position);
//     bondsList.push_back(temporaryBond);
// }
void molecule::addBond(const int index1, const int index2, std::string name)
{
    atom* atom1 = getAtom(index1);
    atom* atom2 = getAtom(index2);

    if (!atom1 || !atom2)
    {
        std::cerr << "Error: Invalid atom indices for bond.\n";
        return;
    }

    std::unique_ptr<bond> newBond = std::make_unique<bond>(name, index1, index2, atom1->getAtomPosition(), atom2->getAtomPosition());
    bondsList.push_back(std::move(newBond));
}
// int molecule::moleculeMass()
// {
//     int m=0;
//     for (auto i = atomsList.begin();i<atomsList.end();++i)
//         m=m+i->getAtomicMass();
//     return m;
// }
int molecule::moleculeMass() const
{
    int m=0;
    for (const auto& thisAtom : atomsList)
    {
        const auto newAtom = dynamic_cast<atom*>(thisAtom.get());
        m=m+newAtom->getAtomicMass();
    }
    return m;
}
// bool molecule::checkValenceLaws(int atomIndex)
// {
//     int valence=getAtom(atomIndex).atomValence();
//     int currentBonds=0;
//     for (const auto& bond: bondsList)
//         if (bond.getAtomIndex1()==atomIndex || bond.getAtomIndex2()==atomIndex)
//             currentBonds++;
//     if (currentBonds<valence)
//         return true;
//     return false;
// }
bool molecule::checkValenceLaws(int atomIndex) const
{
    const int valence=getAtom(atomIndex)->atomValence();
    int currentBonds=0;
    for (const auto& thisBond: bondsList)
    {
        const auto newBond = dynamic_cast<bond*>(thisBond.get());
        if (newBond->getAtomIndex1()==atomIndex || newBond->getAtomIndex2()==atomIndex)
            currentBonds++;
    }
    if (currentBonds<valence)
        return true;
    return false;
}
// int molecule::findAtomAtPosition(const sf::Vector2f& worldPos) const {
//     for (int i = 0; static_cast<size_t>(i) < atomsList.size(); i++) {
//         if (atomsList[i].getBounds().contains(worldPos)) {
//             return i;
//         }
//     }
//     return -1;
// }
int molecule::findAtomAtPosition(const sf::Vector2f& worldPos) const {
    for (int i = 0; static_cast<size_t>(i) < atomsList.size(); i++) {
        if (atomsList[i]->getBounds().contains(worldPos)) {
            return i;
        }
    }
    return -1;
}
// int molecule::findBondPosition(int atomIndex1,int atomIndex2) const
// {
//     int bondIndex=-1;
//     for (const auto& bond:bondsList)
//     {
//         bondIndex++;
//         if ((bond.getAtomIndex1()==atomIndex1 && bond.getAtomIndex2()==atomIndex2)||(bond.getAtomIndex1()==atomIndex2 && bond.getAtomIndex2()==atomIndex1))
//             return bondIndex;
//     }
//     return -1;
// }
int molecule::findBondPosition(int atomIndex1,int atomIndex2) const
{
    int bondIndex=-1;
    for (const auto& ptrBond:bondsList)
    {
        bondIndex++;
        const auto thisBond = dynamic_cast<bond*>(ptrBond.get());
        if ((thisBond->getAtomIndex1()==atomIndex1 && thisBond->getAtomIndex2()==atomIndex2)||(thisBond->getAtomIndex1()==atomIndex2 && thisBond->getAtomIndex2()==atomIndex1))
            return bondIndex;
    }
    return -1;
}
// void molecule::updateBondsPositions()
// {
//     for ( auto& bond: bondsList)
//     {
//         sf::Vector2f pos1 = atomsList[bond.getAtomIndex1()].getAtomPosition();
//         sf::Vector2f pos2 = atomsList[bond.getAtomIndex2()].getAtomPosition();
//         bond.updatePosition(pos1, pos2);
//     }
// }
void molecule::updateBondsPositions() const
{
    for ( auto& ptrBond: bondsList)
    {
        const auto thisBond = dynamic_cast<bond*>(ptrBond.get());
        atom* atom1 = getAtom(thisBond->getAtomIndex1());
        atom* atom2 = getAtom(thisBond->getAtomIndex2());
        thisBond->updatePosition(atom1->getAtomPosition(), atom2->getAtomPosition());
    }
}
void molecule::removeBond(int bondIndex)
{
    if (bondIndex >= 0 && static_cast<size_t>(bondIndex) < bondsList.size())
    {
        bondsList.erase(bondsList.begin() + bondIndex);
    }
}
void molecule::draw(sf::RenderWindow& window) const
{
    for (const auto& bond : bondsList)
        bond->draw(window);
    for (const auto& atom : atomsList)
        atom->draw(window);
}


// atom& molecule::getAtom(int index)
// {
//     //if (static_cast<size_t>(index)<atomsList.size() && index>=0)
//         return atomsList[index];
//     //throw std::out_of_range("Invalid atom's location");
// }
// const atom& molecule::getAtom(int index) const {
//     //if (static_cast<size_t>(index)<atomsList.size() && index>=0)
//         return atomsList[index];
//     //throw std::out_of_range("Invalid atom's location");
// }

atom* molecule::getAtom(size_t index) const
{
    if (index >= atomsList.size())
    {
        throw std::out_of_range("Invalid atom's location");
    }
    return dynamic_cast<atom*>(atomsList[index].get());
}

bond* molecule::getBond(size_t index) const
{
    if (index >= bondsList.size())
    {
        throw std::out_of_range("Invalid bond's location");
    }
    return dynamic_cast<bond*>(bondsList[index].get());
}

// std::ostream& operator<<(std::ostream& out, const molecule& MOLECULE)
// {
//     out<<"Molecula "<<MOLECULE.name<<" contine urmatorii atomi: ";
//     std::vector<atom>::const_iterator i;
//     for (i=MOLECULE.atomsList.begin();i<MOLECULE.atomsList.end()-1;++i)
//     {
//         out<<i->getName()<<", ";
//     }
//     out<<i->getName();
//     out<<"\n";
//     return out;
// }
std::ostream& operator<<(std::ostream& out, const molecule& thisMolecule)
{
    out<<"Molecula "<<thisMolecule.name<<" contine urmatorii atomi: ";
    for (const auto& atom : thisMolecule.atomsList)
        out<<atom->getName()<<" ";
    out<<". ";
    out<<"De asemenea, contine urmatoarele legaturi: ";
    for (const auto& bond : thisMolecule.bondsList)
        out<<bond->getName()<<" ";
    out<<"\n";
    return out;
}