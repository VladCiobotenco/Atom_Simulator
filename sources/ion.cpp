#include <utility>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "ion.hpp"
#include "atom.hpp"

ion::ion(std::string n, const int p, const int g, const int Z, const int m, std::string s, const int c, sf::Font f) :
    atom(std::move(n), p, g, Z, m, std::move(s)), ionCharge(c), chargeFont(std::move(f)), chargeText(sf::Text(chargeFont,"",30))
{
    std::cout << "Un ion a fost creat\n";

    chargeText.setFont(f);
    chargeText.setCharacterSize(14);
    std::string chargeString;
    if (ionCharge > 0)
    {
        if (ionCharge == 1)
            chargeString = "+";
        else
            chargeString = std::to_string(ionCharge) + "+";
        chargeText.setFillColor(sf::Color::Red);
        setAtomOutlineColor(sf::Color::Red);
    }
    else if (ionCharge < 0)
    {
        if (ionCharge == -1)
            chargeString = "-";
        else
            chargeString = std::to_string(ionCharge) + "-";
        chargeText.setFillColor(sf::Color::Blue);
        setAtomOutlineColor(sf::Color::Blue);
    }

    chargeText.setString(chargeString);
    sf::FloatRect textBounds = chargeText.getLocalBounds();
    chargeText.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});
    chargeText.setPosition(getAtomPosition());
}

ion::ion(const ion& other) : atom(other), ionCharge(other.ionCharge), chargeFont(other.chargeFont), chargeText(other.chargeText)
{
    std::cout<<"Un ion a fost distrus\n";
}

ion::~ion()
{
    std::cout<<"Un ion a fost distrus\n";
}

std::shared_ptr<entity> ion::clone() const { return std::make_shared<ion>(*this);}

void ion::draw(sf::RenderWindow& window) const
{
    window.draw(chargeText);
    atom::draw(window);
}

std::ostream& operator<<(std::ostream& os, const ion& ION)
{
    os << static_cast<const atom&>(ION);
    os << "Acest ion are sarcina "<<ION.ionCharge<<"\n";
    return os;
}