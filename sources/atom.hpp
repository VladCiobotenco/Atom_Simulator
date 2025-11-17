#ifndef ATOM_HPP
#define ATOM_HPP

#include <string>
#include <SFML/Graphics.hpp>

#include "entity.hpp"

class atom : public entity
{
    int period, group, atomicNumber, atomicMass;
    std::string symbol;
    sf::CircleShape atomShape;
    sf::Vector2f atomPosition;
public:
    atom(std::string, int, int, int Z, int m, std::string);
    atom(const atom& other);
    atom& operator=(const atom& other);
    ~atom() override;
    std::unique_ptr<entity> clone() const override;

    //[[nodiscard]] const std::string& getName() const;
    void setAtomThickness(float);
    int getAtomicMass() const;
    const std::string& getSymbol() const;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getAtomPosition() const;

    int atomValence() const;
    void draw(sf::RenderWindow& window) const override;
    void move(sf::Vector2f newPosition);
    void restrictAtomToWindow(sf::RenderWindow& window);
    friend std::ostream& operator<<(std::ostream&, const atom&);
};

std::ostream& operator<<(std::ostream& out, const atom& ATOM);

#endif