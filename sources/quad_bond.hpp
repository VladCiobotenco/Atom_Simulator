#ifndef OOP_QUAD_BOND_H
#define OOP_QUAD_BOND_H

#include "bond.hpp"

class quad_bond: public bond
{
    sf::RectangleShape bondLine1;
    sf::RectangleShape bondLine2;
    sf::RectangleShape bondLine3;
    sf::RectangleShape bondLine4;
    float bondLinesOffset;

protected:
    void onDraw(sf::RenderWindow&) const override;
    void bondInfoPrint(std::ostream &) const override;

public:
    quad_bond(int, int);
    quad_bond(const quad_bond &);
    quad_bond& operator=(const quad_bond &);
    ~quad_bond() override;
    std::shared_ptr<entity> clone() const override;

    sf::FloatRect getBounds() const override;
    void updatePosition(const sf::Vector2f &, const sf::Vector2f &) override;
    int getOrder() const override;
};


#endif