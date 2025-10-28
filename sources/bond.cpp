#include "bond.hpp"
#include "atom.hpp"
#include <utility>
#include <SFML/Graphics.hpp>

bond::bond(int atomIndex1, int atomIndex2, std::string  type,const sf::Vector2f& pos1, const sf::Vector2f& pos2): atomIndex1(atomIndex1), atomIndex2(atomIndex2), type(std::move(type)), bondLine(sf::PrimitiveType::Lines, 2)
{
    bondLine[0].position=pos1;
    bondLine[1].position=pos2;
    sf::Color bondColor(80, 80, 80);
    bondLine[0].color=bondColor;
    bondLine[1].color=bondColor;
}
bond::bond(const bond& other): atomIndex1(other.atomIndex1), atomIndex2(other.atomIndex2),type(other.type), bondLine(other.bondLine){}
bond& bond::operator=(const bond& other)
{
    if (this != &other)
    {
        atomIndex1 = other.atomIndex1;
        atomIndex2 = other.atomIndex2;
        type = other.type;
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
    bondLine[0].position=pos1;
    bondLine[1].position=pos2;
}

std::ostream& operator<<(std::ostream& out, const bond& BOND)
{
    out<<"Legatura "<<BOND.type<<" intre atomul cu indexul "<<BOND.atomIndex1<<" si atomul cu indexul "<<BOND.atomIndex2<<"\n";
    return out;
}