#include "atom.hpp"
#include <iostream>
#include <utility>

atom::atom(const int p, const int g, const int Z, const int m, std::string  n, std::string  s): period(p), group(g), atomicNumber(Z), atomicMass(m), name(std::move(n)), symbol(std::move(s)), atomPosition({100.f, 100.f})
{
    std::cout<<"Un atom a fost construit\n";
    float radius;
    if (symbol=="H")
        radius=25.f;
    else radius=50.f;

    atomShape.setRadius(radius);
    atomShape.setOutlineColor(sf::Color::Black);
    atomShape.setOutlineThickness(2.f);
    //atomShape.setOrigin(radius,radius);
    atomShape.setPosition(atomPosition);

    switch (Z)
    {
    case 1: atomShape.setFillColor(sf::Color::White);
        break;
    case 6: atomShape.setFillColor(sf::Color::Black);
        break;
    case 7: atomShape.setFillColor(sf::Color::Blue);
        break;
    case 8: atomShape.setFillColor(sf::Color::Red);
        break;
    default: atomShape.setFillColor(sf::Color::Magenta);
    }

}
atom::atom(const atom& other): period(other.period), group(other.group), atomicNumber(other.atomicNumber), atomicMass(other.atomicMass), name(other.name), symbol(other.symbol), atomShape(other.atomShape), atomPosition(other.atomPosition)
{
    std::cout<<"Un atom a fost copiat - "<<other.name<<"\n";
}
atom& atom::operator=(const atom& other)
{
    if (this != &other)
    {
        period = other.period;
        group = other.group;
        atomicNumber = other.atomicNumber;
        atomicMass = other.atomicMass;
        name = other.name;
        symbol = other.symbol;
        atomShape = other.atomShape;
        atomPosition = other.atomPosition;
    }
    return *this;
}
atom::~atom(){std::cout<<"Un atom a fost distrus\n";}

[[nodiscard]] const std::string& atom::getName() const {return name;}
int atom::getAtomicMass() const {return atomicMass;}


int atom::atomValence() const
{
    if (group==1 || group==2 || group==3)
        return group;
    return 8-group;
}

void atom::draw(sf::RenderWindow& window) const
{
    window.draw(atomShape);
}

void atom::move(sf::Vector2f newPosition)
{
    atomPosition=newPosition;
    atomShape.setPosition(atomPosition);
}

sf::FloatRect atom::getBounds() const
{
    return atomShape.getGlobalBounds();
}

sf::Vector2f atom::getAtomPosition() const
{
    return atomPosition;
}


std::ostream& operator<<(std::ostream& out, const atom& ATOM)
{
    out << "Atomul " << ATOM.name << " cu simbolul " << ATOM.symbol << " se afla in perioada " << ATOM.period << ", grupa principala " << ATOM.group;
    out << " are numarul atomic " << ATOM.atomicNumber << " si masa atomica " << ATOM.atomicMass << "\n";
    return out;
}

