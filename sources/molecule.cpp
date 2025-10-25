#include "molecule.hpp"
#include <iostream> // For std::cout and std::ostream
#include <utility>  // For std::move

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
std::ostream& operator<<(std::ostream& out, molecule& MOLECULE)
{
    out<<"Molecula "<<MOLECULE.name<<" contine urmatorii atomi: ";
    std::vector<atom>::iterator i;
    for (i=MOLECULE.atomsList.begin();i<MOLECULE.atomsList.end()-1;++i)
    {
        out<<i->getName()<<", ";
    }
    out<<i->getName();
    out<<"\n";
    return out;
}