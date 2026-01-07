#ifndef OOP_DOUBLE_BOND_H
#define OOP_DOUBLE_BOND_H

#include "bond.hpp"

class double_bond: public bond{
    sf::RectangleShape bondLine1;
    sf::RectangleShape bondLine2;
    float bondLinesOffset;
    bool isCis;
    bool showConfiguration;

protected:
    void onDraw(sf::RenderWindow&) const override;
    void bondInfoPrint(std::ostream &) const override;

public:
    double_bond(int,int);
    double_bond(const double_bond&);
    double_bond& operator=(const double_bond&);
    ~double_bond() override;
    std::shared_ptr<entity> clone() const override;

    void setVisibility(bool);
    sf::FloatRect getBounds() const override;
    int getOrder() const override;
    //bool getConfiguration() const; // il voi seta mai tarziu pentru a afisa tipul configuratiei la hover

    void updatePosition(const sf::Vector2f&, const sf::Vector2f&) override;
    void toggleConfiguration();
};


#endif