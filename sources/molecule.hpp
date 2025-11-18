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
    std::vector<std::unique_ptr<entity>> atomsList;
    std::vector<std::unique_ptr<entity>> bondsList;
    // std::vector<atom> atomsList;
    // std::vector<bond> bondsList;

public:
    explicit molecule(std::string  n);
    molecule(const molecule& other);
    molecule& operator=(const molecule& other);
    ~molecule();

    void addAtom(const atom&);
    void removeAtom();
    //void addBond(int index1, int index2, const std::string& type);
    void addBond(int, int, const std::string&);
    int moleculeMass() const;
    bool checkValenceLaws(int) const;
    [[nodiscard]] int findAtomAtPosition(const sf::Vector2f&) const;
    [[nodiscard]] int findBondPosition(int, int) const;
    void updateBondsPositions() const;
    void removeBond(int);
    void draw(sf::RenderWindow&) const;

    // [[nodiscard]] atom& getAtom(int index);
    // [[nodiscard]] const atom& getAtom(int index) const;
    atom* getAtom(size_t index) const;
    bond* getBond(size_t index) const;


    friend std::ostream& operator<<(std::ostream&, const molecule&);
};

std::ostream& operator<<(std::ostream& out, const molecule& MOLECULE);

#endif