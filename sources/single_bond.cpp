#include "single_bond.hpp"

single_bond::single_bond(const int atomIndex1, const int atomIndex2)
    : bond("SingleBond", atomIndex1 , atomIndex2)
{
    bondLine.setFillColor(sf::Color(80, 80, 80));
    bondLine.setOrigin({0.f, 2.f});
}

single_bond::single_bond(const single_bond& other): bond(other.getName(), other.atomIndex1, other.atomIndex2), bondLine(other.bondLine){}

single_bond & single_bond::operator=(const single_bond& other)
{
    if (&other != this)
    {
        bond::operator=(other);
        bondLine=other.bondLine;
    }
    return *this;
}

single_bond::~single_bond(){}

std::shared_ptr<entity> single_bond::clone() const
{
    return std::make_shared<single_bond>(*this);
}

void single_bond::onDraw(sf::RenderWindow& thisWindow) const
{
    thisWindow.draw(bondLine);
}

// void single_bond::draw(sf::RenderWindow& thisWindow) const
// {
//     thisWindow.draw(bondLine);
// }

void single_bond::updatePosition(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
    auto [len, angle] = calculateGeometry(p1, p2);
    bondLine.setSize({len, 4.f});
    bondLine.setPosition(p1);
    bondLine.setRotation(sf::degrees(angle));
}

sf::FloatRect single_bond::getBounds() const
{
    return bondLine.getGlobalBounds();
}

int single_bond::getOrder() const {
    return 1;
}


