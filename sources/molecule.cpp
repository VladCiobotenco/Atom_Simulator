#include "molecule.hpp"
#include <iostream>
#include <utility>
#include <stdexcept>

molecule::molecule(std::string  t):name(std::move(t))
{
    std::cout<<"O molecula a fost construita\n";
}
molecule::molecule(const molecule& other): name(other.name)
{
    entitiesList.reserve(other.entitiesList.size());
    for (const auto& entity : other.entitiesList)
        entitiesList.push_back(entity->clone());
}
molecule& molecule::operator=(const molecule& other)
{
    if (this != &other) {
        molecule temp(other);
        std::swap(name, temp.name);
        std::swap(entitiesList, temp.entitiesList);
    }
    return *this;
}
molecule::~molecule(){std::cout<<"O molecula a fost distrusa\n";}

void molecule::addAtom(const atom& tempAtom)
{
    const auto newAtom = std::make_shared<atom>(tempAtom);
    entitiesList.push_back(newAtom);
}

void molecule::addBond(const int index1, const int index2, const std::string& bondName)
{
    if (index1 < 0 || static_cast<size_t>(index1) >entitiesList.size() || index2 < 0 || static_cast<size_t>(index2) > entitiesList.size())
        return;

    const auto atom1 = std::dynamic_pointer_cast<atom>(entitiesList[index1]);
    const auto atom2 = std::dynamic_pointer_cast<atom>(entitiesList[index2]);

    if (atom1 && atom2)
    {
        const auto newBond = std::make_shared<bond>(bondName, index1, index2, atom1->getAtomPosition(), atom2->getAtomPosition());
        entitiesList.push_back(newBond);
        std::cout << "A fost adaugata o legatura intre atomii cu indexul  " << index1 << " si indexul "<< index2<<"\n";
    }
}

void molecule::removeBond(const int bondIndex)
{
    if (bondIndex < 0 || static_cast<size_t>(bondIndex) >= entitiesList.size())
        return;

    if (auto bondPtr = std::dynamic_pointer_cast<bond>(entitiesList[bondIndex]))
    {
        entitiesList.erase(entitiesList.begin() + bondIndex);
        std::cout << "Legatura cu indexul " << bondIndex << " a fost stearsa.\n";
    }
}

void molecule::removeEntity()
{
    entitiesList.pop_back();
}

int molecule::moleculeMass() const
{
    int totalMass=0;
    for (const auto& entityPtr : entitiesList)
        if (const auto thisAtom = std::dynamic_pointer_cast<atom>(entityPtr))
            totalMass = totalMass + thisAtom->getAtomicMass();

    return totalMass;
}

bool molecule::checkValenceLaws(const int atomIndex) const
{

    const auto thisAtom = std::dynamic_pointer_cast<atom>(entitiesList[atomIndex]);
    if (!thisAtom)
        return false;

    const int valence = thisAtom->atomValence();
    int currentBonds = 0;

    for (const auto& entityPtr : entitiesList)
    {
        if (const auto bondPtr = std::dynamic_pointer_cast<bond>(entityPtr))
        {
            if (bondPtr->getAtomIndex1() == atomIndex || bondPtr->getAtomIndex2() == atomIndex)
                currentBonds++;
        }
    }
    return currentBonds < valence;
}

int molecule::findAtomAtPosition(const sf::Vector2f& worldPos) const {
    int entityIndex=0;
    for (const auto& entityPtr : entitiesList)
    {
        if (const auto thisAtom = std::dynamic_pointer_cast<atom>(entityPtr))
            if (thisAtom->getBounds().contains(worldPos))
                return entityIndex;
        entityIndex++;
    }

    return -1;
}

int molecule::findBondPosition(int atomIndex1,int atomIndex2) const
{
    int entityIndex=0;
    for (const auto& entityPtr : entitiesList)
    {
        if (const auto thisBond = std::dynamic_pointer_cast<bond>(entityPtr))
            if ((thisBond->getAtomIndex1()==atomIndex1 && thisBond->getAtomIndex2()==atomIndex2)||(thisBond->getAtomIndex1()==atomIndex2 && thisBond->getAtomIndex2()==atomIndex1))
                return entityIndex;
        entityIndex++;
    }
    return -1;
}

void molecule::updateBondsPositions() const
{
    for ( auto& entityPtr: entitiesList)
        if (const auto bondPtr = std::dynamic_pointer_cast<bond>(entityPtr))
        {
            const auto atom1 = std::dynamic_pointer_cast<atom>(entitiesList[bondPtr->getAtomIndex1()]);
            const auto atom2 = std::dynamic_pointer_cast<atom>(entitiesList[bondPtr->getAtomIndex2()]);
            bondPtr->updatePosition(atom1->getAtomPosition(), atom2->getAtomPosition());
        }
}

void molecule::draw(sf::RenderWindow& window) const
{
    for (const auto& entityPtr : entitiesList)
        if (std::dynamic_pointer_cast<bond>(entityPtr))
            entityPtr->draw(window);

    for (const auto& entityPtr : entitiesList)
        if (!std::dynamic_pointer_cast<bond>(entityPtr))
            entityPtr->draw(window);
}

std::shared_ptr<atom> molecule::getAtom(const size_t index) const
{
    if (index >= entitiesList.size())
        return nullptr;

    return std::dynamic_pointer_cast<atom>(entitiesList[index]);
}

std::ostream& operator<<(std::ostream& out, const molecule& thisMolecule)
{
    out << "Molecula " << thisMolecule.name << " contine urmatorii atomi: ";

    bool firstAtom = true;
    for (const auto& entityPtr : thisMolecule.entitiesList)
    {
        if (const auto atomPtr = std::dynamic_pointer_cast<atom>(entityPtr))
        {
            if (!firstAtom) out << ", ";
            out << atomPtr->getName();
            firstAtom = false;
        }
    }
    if (firstAtom) out << "(niciunul)";

    out << ". De asemenea, contine urmatoarele legaturi: ";

    bool firstBond = true;
    for (const auto& entityPtr : thisMolecule.entitiesList)
    {
        if (const auto bondPtr = std::dynamic_pointer_cast<bond>(entityPtr))
        {
            if (!firstBond) out << ", ";
            out << bondPtr->getName();
            firstBond = false;
        }
    }
    if (firstBond) out << "(niciuna)";

    out << ".\n";
    return out;
}