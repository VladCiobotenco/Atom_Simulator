#ifndef OOP_SINGLE_BOND_H
#define OOP_SINGLE_BOND_H

#include "bond.hpp"


class single_bond: public bond
{
    sf::RectangleShape bondLine;
protected:
    void onDraw(sf::RenderWindow&) const override;
public:
    single_bond(int, int);
    single_bond(const single_bond&);
    single_bond& operator=(const single_bond&);
    ~single_bond() override;
    std::shared_ptr<entity> clone() const override;

    sf::FloatRect getBounds() const override;
    void updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2) override;
    int getOrder() const override;
};


#endif