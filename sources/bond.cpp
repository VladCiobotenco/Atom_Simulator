#include "atom.hpp"
#include "bond.hpp"

#include <cmath>
#include <utility>
#include <SFML/Graphics.hpp>

#ifndef valuePi
#define valuePi 3.14159265358979323846
#endif

bond::bond(std::string name, const int atomIndex1, const int atomIndex2) :
    entity(std::move(name)), atomIndex1(atomIndex1), atomIndex2(atomIndex2){}
bond::bond(const bond& other): entity(other.getName()), atomIndex1(other.atomIndex1), atomIndex2(other.atomIndex2){}
bond& bond::operator=(const bond& other)
{
    if (this != &other)
    {
        entity::operator=(other);
        atomIndex1 = other.atomIndex1;
        atomIndex2 = other.atomIndex2;
    }
    return *this;
}
bond::~bond(){};

int bond::getAtomIndex1() const {return atomIndex1;}
int bond::getAtomIndex2() const {return atomIndex2;}

std::pair<float, float> bond::calculateGeometry(const sf::Vector2f& atomPos1, const sf::Vector2f& atomPos2)
{
    const sf::Vector2f direction = atomPos2 - atomPos1;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    return {length, angle};
}

void bond::draw(sf::RenderWindow& thisWindow) const {
    onDraw(thisWindow);
}

std::ostream& operator<<(std::ostream& out, const bond& thisBond)
{
    out<<"Legatura "<<thisBond.getName()<<" intre atomul cu indexul "<<thisBond.atomIndex1<<" si atomul cu indexul "<<thisBond.atomIndex2<<"\n";
    return out;
}
