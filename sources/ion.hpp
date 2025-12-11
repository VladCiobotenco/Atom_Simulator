#ifndef OOP_ION_H
#define OOP_ION_H

#include "atom.hpp"

class ion final : public atom
{
    int ionCharge;
    sf::Font chargeFont;
    sf::Text chargeText;
    sf::CircleShape chargeHalo;

public:
    ion(std::string, int, int, int Z, int m, std::string, int, sf::Font);
    ion(const ion&);
    ~ion() override;
    std::shared_ptr<entity>clone() const override;

    void onPositionChanged() override;
    void draw(sf::RenderWindow&) const override;
    friend std::ostream& operator<<(std::ostream&, const ion&);
};



#endif