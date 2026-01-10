#include "../include/single_bond.hpp"

#include <cmath>

#include "../include/atom.hpp"

single_bond::single_bond(const int atomIndex1, const int atomIndex2)
    : bond("SingleBond", atomIndex1 , atomIndex2), spinning(false)
{
    bondLine.setFillColor(sf::Color(80, 80, 80));
    bondLine.setOrigin({0.f, 2.f});
}

single_bond::single_bond(const single_bond& other): bond(other.getName(), other.atomIndex1, other.atomIndex2), bondLine(other.bondLine), spinning(other.spinning){}

single_bond & single_bond::operator=(const single_bond& other)
{
    if (&other != this)
    {
        bond::operator=(other);
        bondLine=other.bondLine;
        spinning=other.spinning;
    }
    return *this;
}

single_bond::~single_bond(){}

std::shared_ptr<entity> single_bond::clone() const
{
    return std::make_shared<single_bond>(*this);
}

void single_bond::setSpinning(const bool newSpinning)
{
    spinning = newSpinning;
}

bool single_bond::isSpinning() const
{
    return spinning;
}

void single_bond::spin(const std::shared_ptr<atom>& atom1, const std::shared_ptr<atom> &atom2)
{

    const sf::Vector2f p1 = atom1->getAtomPosition();
    const sf::Vector2f p2 = atom2->getAtomPosition();

    const sf::Vector2f center = (p1 + p2) / 2.f;
    const float theta = 0.0005f;

    auto rotatePoint = [&](sf::Vector2f p)
    {
        const float dx = p.x - center.x;
        const float dy = p.y - center.y;

        const float newX = center.x + (dx * std::cos(theta) - dy * std::sin(theta));
        const float newY = center.y + (dx * std::sin(theta) + dy * std::cos(theta));

        return sf::Vector2f(newX, newY);
    };

    atom1->setPosition(rotatePoint(p1));
    atom2->setPosition(rotatePoint(p2));
    this->updatePosition(rotatePoint(p1),rotatePoint(p2));
}

void single_bond::onDraw(sf::RenderWindow& thisWindow) const
{
    thisWindow.draw(bondLine);
}

void single_bond::bondInfoPrint(std::ostream& out) const
{
    out<<"simpla";
}

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


