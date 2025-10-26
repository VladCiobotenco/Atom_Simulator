#ifndef MOLECULE_HPP
#define MOLECULE_HPP
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "atom.hpp"
#include "bond.hpp"

class molecule
{
    std::string name;
    std::vector<atom> atomsList;
    std::vector<bond> bondsList;

public:
    explicit molecule(std::string  n);
    molecule(const molecule& other);
    molecule& operator=(const molecule& other);
    ~molecule();

    void addAtom(const atom& ATOM);
    void removeAtom();
    void addBond(const bond& BOND);
    void removeBond();
    int moleculeMass();
    int findAtomAtPosition(const sf::Vector2f&)const;
    void draw(sf::RenderWindow&)const;

    [[nodiscard]] atom& getAtom(int index);
    [[nodiscard]] const atom& getAtom(int index) const;

    friend std::ostream& operator<<(std::ostream&, const molecule&);
};

std::ostream& operator<<(std::ostream& out, const molecule& MOLECULE);

#endif