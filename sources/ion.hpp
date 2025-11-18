#ifndef OOP_ION_H
#define OOP_ION_H

#include "atom.hpp"

class ion final : public atom
{
    int ionCharge;
    sf::Font chargeFont;
    sf::Text chargeText;

public:
    ion(std::string, int, int, int Z, int m, std::string, int, sf::Font);
    ion(const ion&);
    ~ion() override;


    void draw(sf::RenderWindow&) const override;
    friend std::ostream& operator<<(std::ostream&, const ion&);
};

std::ostream& operator<<(std::ostream&, const ion&);


#endif