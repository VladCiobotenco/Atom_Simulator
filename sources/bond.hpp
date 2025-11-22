#ifndef BOND_HPP
#define BOND_HPP
#include <string>
#include <SFML/Graphics.hpp>

#include "entity.hpp"

class bond: public entity
{
    int atomIndex1, atomIndex2;
    //std::string type;
    //sf::VertexArray bondLine;
    sf::RectangleShape bondLine;

public:
    bond(std::string, int, int,const sf::Vector2f&, const sf::Vector2f&);
    bond(const bond&);
    bond& operator=(const bond&);
    ~bond() override;
    std::shared_ptr<entity> clone() const override;

    int getAtomIndex1()const;
    int getAtomIndex2()const;

    void draw(sf::RenderWindow&) const override;
    void updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
    sf::FloatRect getBounds() const override;

    friend std::ostream& operator<<(std::ostream&, const bond&);
};

std::ostream& operator<<(std::ostream&, const bond&);

#endif