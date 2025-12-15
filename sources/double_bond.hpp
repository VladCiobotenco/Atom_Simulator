#ifndef OOP_DOUBLE_BOND_H
#define OOP_DOUBLE_BOND_H

#include "bond.hpp"

class double_bond: public bond{
    sf::RectangleShape bondLine1;
    sf::RectangleShape bondLine2;
    float bondLinesOffset;
protected:
    void onDraw(sf::RenderWindow&) const override;
public:
    double_bond(int,int);
    double_bond(const double_bond&);
    double_bond& operator=(const double_bond&);
    ~double_bond() override;
    std::shared_ptr<entity> clone() const override;

    sf::FloatRect getBounds() const override;
    void updatePosition(const sf::Vector2f&, const sf::Vector2f&) override;
    int getOrder() const override;
};


#endif