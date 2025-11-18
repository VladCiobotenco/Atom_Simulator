#include <iostream>
#include <utility>

#include "atom.hpp"
#include "entity.hpp"

atom::atom(std::string  n, const int p, const int g, const int Z, const int m, std::string  s): entity(std::move(n)), period(p), group(g), atomicNumber(Z), atomicMass(m), symbol(std::move(s)), atomPosition({100.f, 100.f})
{
    std::cout<<"Un atom a fost construit - "<<getName()<<"\n";
    float radius;
    if (symbol=="H")
        radius=25.f;
    else radius=50.f;

    atomShape.setRadius(radius);
    atomShape.setOutlineColor(sf::Color::Black);
    atomShape.setOutlineThickness(2.f);
    atomShape.setOrigin({radius,radius});
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
atom::atom(const atom& other): entity(other), period(other.period), group(other.group), atomicNumber(other.atomicNumber),atomicMass(other.atomicMass), symbol(other.symbol), atomShape(other.atomShape), atomPosition(other.atomPosition)
{
    std::cout<<"Un atom a fost copiat - "<<getName()<<"\n";
}
atom& atom::operator=(const atom& other)
{
    if (this != &other)
    {
        entity::operator=(other);

        period = other.period;
        group = other.group;
        atomicNumber = other.atomicNumber;
        atomicMass = other.atomicMass;
        symbol = other.symbol;
        atomShape = other.atomShape;
        atomPosition = other.atomPosition;
    }
    return *this;
}
atom::~atom(){std::cout<<"Un atom a fost distrus - "<<getName()<<"\n";}
std::unique_ptr<entity> atom::clone() const {return std::make_unique<atom>(*this);}

void atom::setAtomOutlineColor(const sf::Color newColor)
{
    atomShape.setOutlineColor(newColor);
}

void atom::setAtomThickness(float thickness)
{
    atomShape.setOutlineThickness(thickness);
}

//[[nodiscard]] const std::string& atom::getName() const {return getName();}
int atom::getAtomicMass() const {return atomicMass;}
const std::string& atom::getSymbol()const {return symbol;}
sf::FloatRect atom::getBounds() const
{
    return atomShape.getGlobalBounds();
}
sf::Vector2f atom::getAtomPosition() const
{
    return atomPosition;
}

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

void atom::restrictAtomToWindow(sf::RenderWindow &window) {
    float windowWidth=window.getSize().x;
    float windowHeight=window.getSize().y;
    float radius=atomShape.getRadius();
    sf::Vector2f currentPosition=atomPosition;

    if (currentPosition.x-radius<0)
        currentPosition.x=radius;
    if (currentPosition.x+radius>windowWidth)
        currentPosition.x=windowWidth-radius;
    if (currentPosition.y-radius<0)
        currentPosition.y=radius;
    if (currentPosition.y+radius>windowHeight)
        currentPosition.y=windowHeight-radius;

    if (currentPosition!=atomPosition)
        move(currentPosition);
}

    std::ostream& operator<<(std::ostream& out, const atom& ATOM)
    {
        out << "Atomul " << ATOM.getName() << " cu simbolul " << ATOM.symbol << " se afla in perioada " << ATOM.period << ", grupa principala " << ATOM.group;
        out << " are numarul atomic " << ATOM.atomicNumber << " si masa atomica " << ATOM.atomicMass << "\n";
        return out;
    }

