#ifndef OOP_SINGLE_BOND_H
#define OOP_SINGLE_BOND_H

#include "bond.hpp"


class single_bond: public bond
{
    sf::RectangleShape bondLine;

public:
    single_bond(int, int);
    explicit single_bond(const single_bond&);
    single_bond& operator=(const single_bond&);
    ~single_bond() override;
    std::shared_ptr<entity> clone() const override;

    //void draw(sf::RenderWindow&) const override;
    void onDraw(sf::RenderWindow&) const override;
    sf::FloatRect getBounds() const override;
    void updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2) override;
    int getOrder() const override;
};


#endif