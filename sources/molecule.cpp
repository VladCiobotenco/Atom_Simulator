#include "molecule.hpp"
#include <iostream>
#include <utility>
#include <stdexcept>

molecule::molecule(std::string  n):name(std::move(n))
{
    std::cout<<"O molecula a fost construita\n";
}
molecule::molecule(const molecule& other): name(other.name), atomsList(other.atomsList), bondsList(other.bondsList){}
molecule& molecule::operator=(const molecule& other)
{
    if (this != &other)
    {
        name = other.name;
        atomsList = other.atomsList;
        bondsList = other.bondsList;
    }
    return *this;
}
molecule::~molecule(){std::cout<<"O molecula a fost distrusa\n";}


void molecule::addAtom(const atom& ATOM)
{
    atomsList.push_back(ATOM);
}
void molecule::removeAtom()
{
    atomsList.pop_back();
}
void molecule::addBond(const bond& BOND)
{
    bondsList.push_back(BOND);
}
void molecule::removeBond()
{
    bondsList.pop_back();
}
int molecule::moleculeMass()
{
    int m=0;
    for (auto i = atomsList.begin();i<atomsList.end();++i)
        m=m+i->getAtomicMass();
    return m;
}
int molecule::findAtomAtPosition(const sf::Vector2f& worldPos) const {
    for (int i = 0; static_cast<size_t>(i) < atomsList.size(); i++) {
        if (atomsList[i].getBounds().contains(worldPos)) {
            return i;
        }
    }
    return -1;
}
void molecule::draw(sf::RenderWindow& window) const
{
    for (const auto& atom : atomsList)
        atom.draw(window);
}


atom& molecule::getAtom(int index)
{
    if (static_cast<size_t>(index)<atomsList.size() && index>=0)
        return atomsList[index];
    return atomsList[0];
    //throw std::out_of_range("Invalid atom's location");
}
const atom& molecule::getAtom(int index) const {
    if (static_cast<size_t>(index)<atomsList.size() && index>=0)
        return atomsList[index];
    return atomsList[0];
    //throw std::out_of_range("Invalid atom's location");
}

std::ostream& operator<<(std::ostream& out, const molecule& MOLECULE)
{
    out<<"Molecula "<<MOLECULE.name<<" contine urmatorii atomi: ";
    std::vector<atom>::const_iterator i;
    for (i=MOLECULE.atomsList.begin();i<MOLECULE.atomsList.end()-1;++i)
    {
        out<<i->getName()<<", ";
    }
    out<<i->getName();
    out<<"\n";
    return out;
}