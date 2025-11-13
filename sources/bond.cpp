#include "bond.hpp"
#include "atom.hpp"

#include <cmath>
#include <utility>
#include <SFML/Graphics.hpp>

#ifndef valuePi
#define valuePi 3.14159265358979323846
#endif

bond::bond(std::string name, int atomIndex1, int atomIndex2, const sf::Vector2f& pos1, const sf::Vector2f& pos2) :
    entity(std::move(name)), atomIndex1(atomIndex1), atomIndex2(atomIndex2)
{
    bondLine.setFillColor(sf::Color(80, 80, 80));
    bondLine.setOrigin({0.f, 4.f / 2.f});
    updatePosition(pos1,pos2);
}
bond::bond(const bond& other): entity(other.getName()), atomIndex1(other.atomIndex1), atomIndex2(other.atomIndex2), bondLine(other.bondLine){}
bond& bond::operator=(const bond& other)
{
    if (this != &other)
    {
        entity::operator=(other);
        atomIndex1 = other.atomIndex1;
        atomIndex2 = other.atomIndex2;
        bondLine=other.bondLine;
    }
    return *this;
}
bond::~bond(){};

int bond::getAtomIndex1() const
{
    return atomIndex1;
}
int bond::getAtomIndex2() const
{
    return atomIndex2;
}


void bond::draw(sf::RenderWindow& window) const {
    window.draw(bondLine);
}
void bond::updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
    sf::Vector2f direction = pos1 - pos2;
    float len = sqrt(direction.x*direction.x+direction.y*direction.y);
    float angle = std::atan2(direction.y, direction.x) * 180.f / valuePi;

    bondLine.setSize({len, 4.f});
    bondLine.setPosition(pos1);
    bondLine.setRotation(sf::degrees(angle+180.f));
}
sf::FloatRect bond::getBounds() const
{
    return bondLine.getGlobalBounds();
}

std::ostream& operator<<(std::ostream& out, const bond& BOND)
{
    out<<"Legatura "<<BOND.getName()<<" intre atomul cu indexul "<<BOND.atomIndex1<<" si atomul cu indexul "<<BOND.atomIndex2<<"\n";
    return out;
}