#ifndef OOP_DOUBLE_BOND_H
#define OOP_DOUBLE_BOND_H

#include "bond.hpp"

class double_bond: public bond{
    sf::RectangleShape bondLine1;
    sf::RectangleShape bondLine2;
    float bondLinesOffset, bondLinesThickness;
public:
    double_bond(int,int);
    double_bond(const double_bond&);
    double_bond& operator=(const double_bond&);
    ~double_bond() override;
    std::shared_ptr<entity> clone() const override;

    void draw(sf::RenderWindow&) const override;
    sf::FloatRect getBounds() const override;
    void updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2) override;
    int getOrder() const override;

};


#endif