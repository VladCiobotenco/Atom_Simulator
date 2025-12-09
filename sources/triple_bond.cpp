#include "triple_bond.hpp"

triple_bond::triple_bond(int atomIndex1, int atomIndex2) : bond("triple_bond",atomIndex1, atomIndex2), bondLinesOffset(3.f)
{
    bondLine1.setFillColor(sf::Color(80, 80, 80));
    bondLine1.setOrigin({0.f, 2.f - 2*bondLinesOffset});
    bondLine2.setFillColor(sf::Color(80, 80, 80));
    bondLine2.setOrigin({0.f, 2.f});
    bondLine3.setFillColor(sf::Color(80, 80, 80));
    bondLine3.setOrigin({0.f, 2.f + 2*bondLinesOffset});
}

triple_bond::triple_bond(const triple_bond &other): bond(other.getName(),other.atomIndex1, other.atomIndex2),bondLine1(other.bondLine1),bondLine2(other.bondLine2),bondLine3(other.bondLine3), bondLinesOffset(other.bondLinesOffset){}

triple_bond& triple_bond::operator=(const triple_bond &other)
{
    if (&other !=this)
    {
        bond::operator=(other);
        bondLine1 = other.bondLine1;
        bondLine2 = other.bondLine2;
        bondLine3 = other.bondLine3;
        bondLinesOffset = other.bondLinesOffset;
    }
    return *this;
}

triple_bond::~triple_bond() {}

std::shared_ptr<entity> triple_bond::clone() const
{
    return std::make_shared<triple_bond>(*this);
}

void triple_bond::draw(sf::RenderWindow& window) const
{
    window.draw(bondLine1);
    window.draw(bondLine2);
    window.draw(bondLine3);
}

sf::FloatRect triple_bond::getBounds() const
{
    const sf::FloatRect r1 = bondLine1.getGlobalBounds();
    const sf::FloatRect r2 = bondLine2.getGlobalBounds();
    const sf::FloatRect r3 = bondLine3.getGlobalBounds();

    float left = std::min(std::min(r1.position.x, r2.position.x),r3.position.x);
    float top = std::min(std::min(r1.position.y, r2.position.y),r3.position.y);

    const float right = std::max(std::max(r1.position.x + r1.size.x, r2.position.x + r2.size.x),r3.position.x + r3.size.x);
    const float bottom = std::max(std::max(r1.position.y + r1.size.y, r2.position.y + r2.size.y),r3.position.y + r3.size.y);

    return {{left, top},{right-left,bottom-top}};
}

void triple_bond::updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
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
}

int triple_bond::getOrder() const
{
    return 3;
}



