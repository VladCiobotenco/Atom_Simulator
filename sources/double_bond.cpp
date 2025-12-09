#include "double_bond.hpp"

double_bond::double_bond(int atomIndex1, int atomIndex2): bond("double_bond", atomIndex1, atomIndex2), bondLinesOffset(3.f)
{
    bondLine1.setFillColor(sf::Color(80, 80, 80));
    bondLine1.setOrigin({0.f, 2.f + bondLinesOffset});
    bondLine2.setFillColor(sf::Color(80, 80, 80));
    bondLine2.setOrigin({0.f, 2.f - bondLinesOffset});
}

double_bond::double_bond(const double_bond& other): bond(other.getName(),other.atomIndex1, other.atomIndex2), bondLine1(other.bondLine1),bondLine2(other.bondLine2), bondLinesOffset(other.bondLinesOffset){}

double_bond& double_bond::operator=(const double_bond& other)
{
    if (this!=&other){
        bond::operator=(other);
        bondLine1=other.bondLine1;
        bondLine2=other.bondLine2;
        bondLinesOffset=other.bondLinesOffset;
    }
    return *this;
}

double_bond::~double_bond() {}

std::shared_ptr<entity> double_bond::clone() const
{
    return std::make_shared<double_bond>(*this);
}

void double_bond::onDraw(sf::RenderWindow& thisWindow) const
{
    thisWindow.draw(bondLine1);
    thisWindow.draw(bondLine2);
}

// void double_bond::draw(sf::RenderWindow& window) const
// {
//     window.draw(bondLine1);
//     window.draw(bondLine2);
// }

sf::FloatRect double_bond::getBounds() const
{
    const sf::FloatRect r1 = bondLine1.getGlobalBounds();
    const sf::FloatRect r2 = bondLine2.getGlobalBounds();

    float left = std::min(r1.position.x, r2.position.x);
    float top = std::min(r1.position.y, r2.position.y);

    const float right = std::max(r1.position.x + r1.size.x, r2.position.x + r2.size.x);
    const float bottom = std::max(r1.position.y + r1.size.y, r2.position.y + r2.size.y);

    return sf::FloatRect({left, top},{right - left, bottom - top});
}

void double_bond::updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2)
{
    auto [length, angle] = calculateGeometry(pos1, pos2);

    bondLine1.setSize({length, 4.0});
    bondLine1.setPosition(pos1);
    bondLine1.setRotation(sf::degrees(angle));

    bondLine2.setSize({length, 4.0});
    bondLine2.setPosition(pos1);
    bondLine2.setRotation(sf::degrees(angle));
}

int double_bond::getOrder() const
{
    return 2;
}


