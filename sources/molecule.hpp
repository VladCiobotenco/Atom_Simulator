#ifndef MOLECULE_HPP
#define MOLECULE_HPP
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "atom.hpp"
#include "bond.hpp"

class molecule
{
    std::string name;
    std::vector<std::shared_ptr<entity>> entitiesList;

public:
    explicit molecule(std::string);
    molecule(const molecule&);
    molecule& operator=(const molecule&);
    ~molecule();

    void addAtom(const std::shared_ptr<atom>&, const sf::Vector2f&);
    void addBond(int, int, const std::string&);
    void removeEntity(int);
    void removeEntities();

    [[nodiscard]] int getMoleculeMass() const;
    [[nodiscard]] std::string getMolecularFormula() const;

    [[nodiscard]] int checkValenceLaws(int) const;
    [[nodiscard]] bool checkAtomsConnections() const;

    [[nodiscard]] int findAtomAtPosition(const sf::Vector2f&) const;
    [[nodiscard]] int findBondPosition(int, int) const;

    void updateBondsPositions() const;
    void updateBondsIndices(int) const;
    void draw(sf::RenderWindow&) const;


    [[nodiscard]] std::shared_ptr<atom> getAtom(size_t index) const;


    friend std::ostream& operator<<(std::ostream&, const molecule&);
};


#endif