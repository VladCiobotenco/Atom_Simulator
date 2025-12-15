#ifndef OOP_TRIPLE_BOND_H
#define OOP_TRIPLE_BOND_H

#include "bond.hpp"

class triple_bond: public bond
{
    sf::RectangleShape bondLine1;
    sf::RectangleShape bondLine2;
    sf::RectangleShape bondLine3;
    float bondLinesOffset;
protected:
    void onDraw(sf::RenderWindow&) const override;
public:
    triple_bond(int, int);
    triple_bond(const triple_bond &);
    triple_bond& operator=(const triple_bond &);
    ~triple_bond() override;
    std::shared_ptr<entity> clone() const override;

    sf::FloatRect getBounds() const override;
    void updatePosition(const sf::Vector2f &, const sf::Vector2f &) override;
    int getOrder() const override;
};


#endif