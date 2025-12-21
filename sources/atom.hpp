#ifndef ATOM_HPP
#define ATOM_HPP

#include <string>
#include <SFML/Graphics.hpp>

#include "entity.hpp"

class atom : public entity
{
protected:
    int period, group, atomicNumber, atomicMass;
    std::string symbol;
    sf::CircleShape atomShape;

    std::vector<sf::CircleShape>electrons;
    bool showElectrons;
    int availableElectrons;

public:
    atom(std::string, int, int, int, int, std::string);
    atom(const atom& other);
    atom& operator=(const atom& other);
    ~atom() override;
    std::shared_ptr<entity> clone() const override;

    void setAtomOutlineColor(sf::Color);
    void setAtomThickness(float);

    int getAtomicMass() const;
    const std::string& getSymbol() const;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getAtomPosition() const;

    void onPositionChanged() override;
    int atomValence() const;
    void restrictAtomToBounds(const sf::FloatRect&);

    void setShowElectrons(bool);
    void setAvailableElectrons(int);

    void draw(sf::RenderWindow&) const override;
    friend std::ostream& operator<<(std::ostream&, const atom&);
};

#endif