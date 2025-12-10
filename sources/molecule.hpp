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
    explicit molecule(std::string  n);
    molecule(const molecule& other);
    molecule& operator=(const molecule& other);
    ~molecule();

    void addAtom(std::shared_ptr<atom>, const sf::Vector2f&);
    void addBond(int, int, const std::string&);
    void removeEntity(int);
    void removeEntities();
    [[nodiscard]] int moleculeMass() const;
    [[nodiscard]] int checkValenceLaws(int) const;
    [[nodiscard]] int findAtomAtPosition(const sf::Vector2f&) const;
    [[nodiscard]] int findBondPosition(int, int) const;
    void updateBondsPositions() const;
    void draw(sf::RenderWindow&) const;

    [[nodiscard]] std::shared_ptr<atom> getAtom(size_t index) const;
    //[[nodiscard]] bond* getBond(size_t index) const;


    friend std::ostream& operator<<(std::ostream&, const molecule&);
};

std::ostream& operator<<(std::ostream&, const molecule&);

#endif