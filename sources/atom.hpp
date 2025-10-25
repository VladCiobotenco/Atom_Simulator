#ifndef ATOM_HPP
#define ATOM_HPP

#include <iostream>
#include <string>
#include <utility>
#include <SFML/Graphics.hpp>

class atom
{
    int period, group, atomicNumber, atomicMass;
    std::string name, symbol;
    sf::CircleShape atomShape;
public:
    atom(const int p, const int g, const int Z, const int m, std::string  n, std::string  s);
    atom(const atom& other);
    atom& operator=(const atom& other);
    ~atom();
    [[nodiscard]] const std::string& getName() const;
    int getAtomicMass() const;
    int atomValence() const;
    void draw(sf::RenderWindow& window) const;
    friend std::ostream& operator<<(std::ostream&, const atom&);
};

std::ostream& operator<<(std::ostream& out, const atom& ATOM);

#endif