#include "quad_bond.hpp"

quad_bond::quad_bond(int atomIndex1, int atomIndex2) : bond("quad_bond",atomIndex1, atomIndex2), bondLinesOffset(3.f)
{
    bondLine1.setFillColor(sf::Color(80, 80, 80));
    bondLine1.setOrigin({0.f, 2.f - 3*bondLinesOffset});
    bondLine2.setFillColor(sf::Color(80, 80, 80));
    bondLine2.setOrigin({0.f, 2.f - bondLinesOffset});
    bondLine3.setFillColor(sf::Color(80, 80, 80));
    bondLine3.setOrigin({0.f, 2.f + bondLinesOffset});
    bondLine4.setFillColor(sf::Color(80, 80, 80));
    bondLine4.setOrigin({0.f, 2.f + 3*bondLinesOffset});
}

quad_bond::quad_bond(const quad_bond &other): bond(other.getName(),other.atomIndex1, other.atomIndex2),bondLine1(other.bondLine1),bondLine2(other.bondLine2),bondLine3(other.bondLine3), bondLine4(other.bondLine4), bondLinesOffset(other.bondLinesOffset){}

quad_bond& quad_bond::operator=(const quad_bond &other)
{
    if (&other !=this)
    {
        bond::operator=(other);
        bondLine1 = other.bondLine1;
        bondLine2 = other.bondLine2;
        bondLine3 = other.bondLine3;
        bondLine4 = other.bondLine4;
        bondLinesOffset = other.bondLinesOffset;
    }
    return *this;
}

quad_bond::~quad_bond() {}

std::shared_ptr<entity> quad_bond::clone() const
{
    return std::make_shared<quad_bond>(*this);
}

void quad_bond::onDraw(sf::RenderWindow& thisWindow) const
{
    thisWindow.draw(bondLine1);
    thisWindow.draw(bondLine2);
    thisWindow.draw(bondLine3);
    thisWindow.draw(bondLine4);
}

void quad_bond::bondInfoPrint(std::ostream& out) const
{
    out<<"cvadrupla";
}

sf::FloatRect quad_bond::getBounds() const
{
    const sf::FloatRect r1 = bondLine1.getGlobalBounds();
    const sf::FloatRect r2 = bondLine2.getGlobalBounds();
    const sf::FloatRect r3 = bondLine3.getGlobalBounds();
    const sf::FloatRect r4 = bondLine4.getGlobalBounds();

    float left = std::min(std::min(std::min(r1.position.x, r2.position.x),r3.position.x),r4.position.x);
    float top = std::min(std::min(std::min(r1.position.y, r2.position.y),r3.position.y),r4.position.x);

    const float right = std::max(std::max(std::max(r1.position.x + r1.size.x, r2.position.x + r2.size.x),r3.position.x + r3.size.x),r4.position.x+r4.size.x);
    const float bottom = std::max(std::max(std::max(r1.position.y + r1.size.y, r2.position.y + r2.size.y),r3.position.y + r3.size.y),r4.position.y+r4.size.y);

    return {{left, top},{right-left,bottom-top}};
}

void quad_bond::updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
    auto [length, angle]=calculateGeometry(pos1, pos2);

    bondLine1.setSize({length,4.0});
    bondLine1.setPosition(pos1);
    bondLine1.setRotation(sf::degrees(angle));

    bondLine2.setSize({length,4.0});
    bondLine2.setPosition(pos1);
    bondLine2.setRotation(sf::degrees(angle));

    bondLine3.setSize({length,4.0});
    bondLine3.setPosition(pos1);
    bondLine3.setRotation(sf::degrees(angle));

    bondLine4.setSize({length,4.0});
    bondLine4.setPosition(pos1);
    bondLine4.setRotation(sf::degrees(angle));
}

int quad_bond::getOrder() const
{
    return 4;
}



