#include "molecule.hpp"
#include <iostream>
#include <map>
#include <utility>

#include "double_bond.hpp"
#include "exceptions.hpp"
#include "quad_bond.hpp"
#include "single_bond.hpp"
#include "triple_bond.hpp"

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

void molecule::addAtom(std::shared_ptr<atom> tempAtom, const sf::Vector2f& spawnPosition)
{
    tempAtom->setPosition(spawnPosition);
    entitiesList.push_back(tempAtom);
}

void molecule::addBond(const int index1, const int index2, const std::string& bondName)
{
    if (index1 == index2)
    {
        throw chemistryLawsException("Nu se poate lega un atom cu el insusi(indexul " + std::to_string(index1) + ")\n");
    }

    if (index1 < 0 || static_cast<size_t>(index1) >entitiesList.size() || index2 < 0 || static_cast<size_t>(index2) > entitiesList.size())
        return;

    const auto atom1 = std::dynamic_pointer_cast<atom>(entitiesList[index1]);
    const auto atom2 = std::dynamic_pointer_cast<atom>(entitiesList[index2]);

    if (atom1 && atom2)
    {
        if (bondName == "single_bond")
        {
            const auto newBond = std::make_shared<single_bond>(index1, index2);
            entitiesList.push_back(newBond);
            std::cout << "A fost adaugata o legatura simpla intre atomii cu indexul  " << index1 << " si indexul "<< index2<<"\n";
        }
        else if (bondName == "double_bond")
        {
            const auto newBond = std::make_shared<double_bond>(index1, index2);
            entitiesList.push_back(newBond);
            std::cout << "A fost adaugata o legatura dubla intre atomii cu indexul  " << index1 << " si indexul "<< index2<<"\n";
        }

        else if (bondName == "triple_bond")
        {
            const auto newBond = std::make_shared<triple_bond>(index1, index2);
            entitiesList.push_back(newBond);
            std::cout << "A fost adaugata o legatura tripla intre atomii cu indexul  " << index1 << " si indexul "<< index2<<"\n";
        }

        else if (bondName == "quad_bond")
        {
            const auto newBond = std::make_shared<quad_bond>(index1, index2);
            entitiesList.push_back(newBond);
            std::cout << "A fost adaugata o legatura quad intre atomii cu indexul  " << index1 << " si indexul "<< index2<<"\n";
        }

        else throw chemistryLawsException("Nu exista tipul de legatura");
    }

    int newCount = checkValenceLaws(index1);
    atom1->setAvailableElectrons(newCount);
    newCount=checkValenceLaws(index2);
    atom2->setAvailableElectrons(newCount);
}

void molecule::removeEntity(int index)
{
    if (auto bondPtr = std::dynamic_pointer_cast<bond>(entitiesList[index]))
    {
        entitiesList.erase(entitiesList.begin() + index);
        std::cout << "Legatura cu indexul " << index << " a fost stearsa.\n";

        int index1=bondPtr->getAtomIndex1();
        int index2=bondPtr->getAtomIndex2();
        const auto atom1 = std::dynamic_pointer_cast<atom>(entitiesList[index1]);
        const auto atom2 = std::dynamic_pointer_cast<atom>(entitiesList[index2]);

        int newCount = checkValenceLaws(index1);
        atom1->setAvailableElectrons(newCount);
        newCount=checkValenceLaws(index2);
        atom2->setAvailableElectrons(newCount);
    }
    else if (auto atomPtr = std::dynamic_pointer_cast<atom>(entitiesList[index]))
    {
        int i=0;
        for (const auto& entityPtr : entitiesList)
        {
            if (auto bondPtr = std::dynamic_pointer_cast<bond>(entityPtr))
                if (bondPtr->getAtomIndex1()==index || bondPtr->getAtomIndex2()==index)
                    removeEntity(i);
            i++;
        }

        entitiesList.erase(entitiesList.begin() + index);
        std::cout << "Atomul cu indexul " << index << " a fost sters.\n";
    }

}

void molecule::removeEntities()
{
    while (!entitiesList.empty())
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

std::string molecule::getMolecularFormula() const
{
    if (entitiesList.empty()) return "";

    std::map<std::string, int> counts;

    for (const auto& entity : entitiesList)
    {
        if (auto atomPtr = std::dynamic_pointer_cast<atom>(entity))
            counts[atomPtr->getSymbol()]++;
    }

    std::stringstream ss;
    if (counts.count("C")) {
        ss << "C";
        if (counts["C"] > 1)
            ss << counts["C"];
        counts.erase("C");
    }

    if (counts.count("H")) {
        ss << "H";
        if (counts["H"] > 1)
            ss << counts["H"];
        counts.erase("H");
    }

    for (const auto& pair : counts)
    {
        ss << pair.first;
        if (pair.second > 1)
            ss << pair.second;
    }

    return ss.str();
}

int molecule::checkValenceLaws(const int atomIndex) const
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
                currentBonds+=bondPtr->getOrder();
        }
    }
    return valence-currentBonds;
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