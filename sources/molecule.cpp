#include "../include/molecule.hpp"
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <utility>

#include "../include/double_bond.hpp"
#include "../include/exceptions.hpp"
#include "../include/quad_bond.hpp"
#include "../include/single_bond.hpp"
#include "../include/triple_bond.hpp"
#include "../include/bond_factory.hpp"

molecule::molecule(std::string  t):name(std::move(t))
{
    std::cout<<"O molecula a fost construita\n";
}
molecule::molecule(const molecule& other): name(other.name)
{
    atomsList.reserve(other.atomsList.size());
    for (const auto& atom: other.atomsList)
        atomsList.push_back(atom->clone());
    bondsList.reserve(other.bondsList.size());
    for (const auto& bond: other.bondsList)
        bondsList.push_back(bond->clone());
}
molecule& molecule::operator=(const molecule& other)
{
    if (this != &other) {
        molecule temp(other);
        std::swap(name, temp.name);
        std::swap(atomsList, temp.atomsList);
        std::swap(bondsList, temp.bondsList);
    }
    return *this;
}
molecule::~molecule(){std::cout<<"O molecula a fost distrusa\n";}

void molecule::addAtom(const std::shared_ptr<atom>& tempAtom, const sf::Vector2f& spawnPosition)
{
    tempAtom->setPosition(spawnPosition);
    atomsList.push_back(tempAtom);
}

void molecule::addBond(const int index1, const int index2, const std::string& bondName)
{
    if (index1 == index2)
    {
        throw chemistryLawsException("Nu se poate lega un atom cu el insusi(indexul " + std::to_string(index1) + ")\n");
    }

    if (index1 < 0 || static_cast<size_t>(index1) >atomsList.size() || index2 < 0 || static_cast<size_t>(index2) > atomsList.size())
        return;

    const auto atom1 = std::static_pointer_cast<atom>(atomsList[index1]);
    const auto atom2 = std::static_pointer_cast<atom>(atomsList[index2]);

    if (atom1 && atom2)                                                                                                             /// Va fi creat o clasa factory pentru bonds
    {
        const auto newBond = bond_factory::createBond(bondName, index1, index2);
        bondsList.push_back(newBond);
        std::cout << "A fost adaugata o legatura de tip \""<<bondName<<"\" intre atomii cu indexul  " << index1 << " si indexul "<< index2<<"\n";
    }

    int newCount = checkValenceLaws(index1);
    atom1->setAvailableElectrons(newCount);
    newCount=checkValenceLaws(index2);
    atom2->setAvailableElectrons(newCount);
}

void molecule::removeAtom(int index)
{
    for (int i = (int)bondsList.size() - 1; i >= 0; --i)
    {
        auto bondPtr = std::static_pointer_cast<bond>(bondsList[i]);
        if (bondPtr->getAtomIndex1() == index || bondPtr->getAtomIndex2() == index)
            removeBond(i);
    }
    atomsList.erase(atomsList.begin() + index);
    std::cout << "Atomul cu indexul " << index << " a fost sters.\n";
    updateBondsIndices(index);
}

void molecule::removeBond(int index)
{
    auto bondPtr = std::static_pointer_cast<bond>(bondsList[index]);
    int index1=bondPtr->getAtomIndex1();
    int index2=bondPtr->getAtomIndex2();

    std::cout << "Legatura cu indexul " << index << " a fost stearsa.\n";
    bondsList.erase(bondsList.begin() + index);

    if (static_cast<size_t>(index1) < atomsList.size())
    {
        auto atom1 = std::static_pointer_cast<atom>(atomsList[index1]);
        if (atom1) {
            const int newCount = checkValenceLaws(index1);
            atom1->setAvailableElectrons(newCount);
        }
    }

    if (static_cast<size_t>(index2) < atomsList.size())
    {
        auto atom2 = std::static_pointer_cast<atom>(atomsList[index2]);
        if (atom2) {
            const int newCount = checkValenceLaws(index2);
            atom2->setAvailableElectrons(newCount);
        }
    }
}

void molecule::removeEntities()
{
    while (!atomsList.empty())
        atomsList.pop_back();
    while (!bondsList.empty())
        bondsList.pop_back();
}

int molecule::getMoleculeMass() const
{
    int totalMass=0;

    for (const auto& atomPtr : atomsList)
    {
        const auto& thisAtom = std::static_pointer_cast<atom>(atomPtr);
        totalMass = totalMass + thisAtom->getAtomicMass();
    }

    return totalMass;
}

std::string molecule::getMolecularFormula() const
{
    if ((atomsList.empty() && bondsList.empty()) || checkAtomsConnections() == false)
        return "";

    std::map<std::string, int> counts;

    for (const auto& entity : atomsList)
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
    const auto thisAtom = std::static_pointer_cast<atom>(atomsList[atomIndex]);
    if (!thisAtom)
        return false;

    const int valence = thisAtom->atomValence();
    int currentBonds = 0;

    for (const auto& entityPtr : bondsList)
    {
        const auto bondPtr = std::static_pointer_cast<bond>(entityPtr);
        if (bondPtr->getAtomIndex1() == atomIndex || bondPtr->getAtomIndex2() == atomIndex)
            currentBonds+=bondPtr->getOrder();
    }
    return valence-currentBonds;
}

bool molecule::checkAtomsConnections() const {

    int atomCount = 0, startAtomIndex = 0;
    atomCount = atomsList.size();

    std::map<int, std::vector<int>> adjacencyList;

    for (const auto& entityPtr : bondsList)
    {
        auto bondPtr = std::static_pointer_cast<bond>(entityPtr);
        adjacencyList[bondPtr -> getAtomIndex1()].push_back(bondPtr -> getAtomIndex2());
        adjacencyList[bondPtr -> getAtomIndex2()].push_back(bondPtr -> getAtomIndex1());
    }

    std::queue<int> q;
    std::set<int> visited;
    q.push(startAtomIndex);
    visited.insert(startAtomIndex);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : adjacencyList[u])
        {
            if (!visited.contains(v))
            {
                visited.insert(v);
                q.push(v);
            }
        }
    }

    if (visited.size() == static_cast<size_t>(atomCount))
        return true;

    return false;
}

bool molecule::checkHydrocarbon() const
{
    bool containsC = false, containsH = false;
    for (const auto& entityPtr : atomsList)
    {
        auto thisAtom = std::static_pointer_cast<atom>(entityPtr);
        if (thisAtom->getSymbol() == "C")
            containsC = true;
        if (thisAtom->getSymbol() == "H")
            containsH = true;
    }
    return containsC && containsH;
}

int molecule::findAtomAtPosition(const sf::Vector2f& worldPos) const
{
    int entityIndex=0;
    for (const auto& entityPtr : atomsList)
    {
        const auto thisAtom = std::static_pointer_cast<atom>(entityPtr);
        if (thisAtom->getBounds().contains(worldPos))
            return entityIndex;
        entityIndex++;
    }

    return -1;
}

int molecule::findBondAtPosition(const sf::Vector2f& mousePos) const
{
    for (size_t i = 0; i < bondsList.size(); ++i)
    {
        if (bondsList[i]->getBounds().contains(mousePos))
            return static_cast<int>(i);
    }
    return -1;
}


int molecule::findBondPosition(int atomIndex1,int atomIndex2) const
{
    int entityIndex=0;
    for (const auto& entityPtr : bondsList)
    {
        const auto thisBond = std::static_pointer_cast<bond>(entityPtr);
        if ((thisBond->getAtomIndex1()==atomIndex1 && thisBond->getAtomIndex2()==atomIndex2)||(thisBond->getAtomIndex1()==atomIndex2 && thisBond->getAtomIndex2()==atomIndex1))
            return entityIndex;
        entityIndex++;
    }
    return -1;
}

void molecule::updateBondsPositions() const
{
    for ( auto& entityPtr: bondsList)
    {
        const auto bondPtr = std::static_pointer_cast<bond>(entityPtr);
        const auto atom1 = std::static_pointer_cast<atom>(atomsList[bondPtr->getAtomIndex1()]);
        const auto atom2 = std::static_pointer_cast<atom>(atomsList[bondPtr->getAtomIndex2()]);
        bondPtr->updatePosition(atom1->getAtomPosition(), atom2->getAtomPosition());
    }
}

void molecule::updateBondsIndices(int removedIndex) const
{
    for (auto& entityPtr: bondsList)
    {
        auto bondPtr = std::static_pointer_cast<bond>(entityPtr);
        int idx1 = bondPtr->getAtomIndex1();
        int idx2 = bondPtr->getAtomIndex2();
        bool changed = false;
        if (idx1 > removedIndex)
        {
            idx1--;
            changed = true;
        }
        if (idx2 > removedIndex)
        {
            idx2--;
            changed = true;
        }

        if (changed)
            bondPtr->setIndices(idx1, idx2);
        }
}

void molecule::draw(sf::RenderWindow& window) const
{
    for (const auto& entityPtr : bondsList)
            entityPtr->draw(window);

    for (const auto& entityPtr : atomsList)
            entityPtr->draw(window);
}

std::shared_ptr<atom> molecule::getAtom(const size_t index) const
{
    if (index >= atomsList.size())
        return nullptr;

    return std::static_pointer_cast<atom>(atomsList[index]);
}

std::shared_ptr<bond> molecule::getBond(size_t index) const
{
    if (index >= bondsList.size())
        return nullptr;

    return std::static_pointer_cast<bond>(bondsList[index]);
}

std::ostream& operator<<(std::ostream& out, const molecule& thisMolecule)
{
    out << "Molecula " << thisMolecule.name << " contine urmatorii atomi: ";

    bool firstAtom = true;
    for (const auto& entityPtr : thisMolecule.atomsList)
    {
        const auto atomPtr = std::static_pointer_cast<atom>(entityPtr);
        if (!firstAtom)
            out << ", ";
        out << atomPtr->getName();
            firstAtom = false;
    }

    if (firstAtom) out << "(niciunul)";

    out << ". De asemenea, contine urmatoarele legaturi: ";

    bool firstBond = true;
    for (const auto& entityPtr : thisMolecule.bondsList)
    {
        const auto bondPtr = std::static_pointer_cast<bond>(entityPtr);
        if (!firstBond)
            out << ", ";
        out << bondPtr->getName();
        firstBond = false;
    }
    if (firstBond) out << "(niciuna)";

    out << ".\n";
    return out;
}
