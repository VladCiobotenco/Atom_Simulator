#ifndef OOP_SINGLE_BOND_H
#define OOP_SINGLE_BOND_H

#include "atom.hpp"
#include "bond.hpp"


class single_bond: public bond
{
    sf::RectangleShape bondLine;
    bool spinning;
protected:
    void onDraw(sf::RenderWindow&) const override;
    void bondInfoPrint(std::ostream &) const override;
public:
    single_bond(int, int);
    single_bond(const single_bond&);
    single_bond& operator=(const single_bond&);
    ~single_bond() override;
    std::shared_ptr<entity> clone() const override;

    void toggleSpin();
    bool isSpinning() const;
    void spin(const std::shared_ptr<atom>&, const std::shared_ptr<atom> &);
    sf::FloatRect getBounds() const override;
    void updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2) override;
    int getOrder() const override;
};


#endif