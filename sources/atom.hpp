#ifndef ATOM_HPP
#define ATOM_HPP

#include <string>
#include <SFML/Graphics.hpp>

class atom
{
    int period, group, atomicNumber, atomicMass;
    std::string name, symbol;
    sf::CircleShape atomShape;
    sf::Vector2f atomPosition;
public:
    atom(const int p, const int g, const int Z, const int m, std::string  n, std::string  s);
    atom(const atom& other);
    atom& operator=(const atom& other);
    ~atom();
    [[nodiscard]] const std::string& getName() const;
    int getAtomicMass() const;
    std::string getSymbol() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getAtomPosition() const;
    int atomValence() const;
    void draw(sf::RenderWindow& window) const;
    void move(sf::Vector2f newPosition);
    void restrictAtomToWindow(sf::RenderWindow& window);
    friend std::ostream& operator<<(std::ostream&, const atom&);
};

std::ostream& operator<<(std::ostream& out, const atom& ATOM);

#endif