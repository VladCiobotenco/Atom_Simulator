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
    std::shared_ptr<entity> clone() const override;

    void setAtomOutlineColor(sf::Color);
    //[[nodiscard]] const std::string& getName() const;
    void setAtomThickness(float);
    void setAtomPosition(sf::Vector2f);
    int getAtomicMass() const;
    const std::string& getSymbol() const;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getAtomPosition() const;

    int atomValence() const;
    void draw(sf::RenderWindow&) const override;
    void move(sf::Vector2f);
    void restrictAtomToBounds(sf::FloatRect&);
    friend std::ostream& operator<<(std::ostream&, const atom&);
};

std::ostream& operator<<(std::ostream&, const atom&);

#endif