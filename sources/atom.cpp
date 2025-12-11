#include <iostream>
#include <utility>

#include "atom.hpp"
#include "entity.hpp"

#include "exceptions.hpp"
#include "../libraries/json.hpp"

atom::atom(std::string  n, const int p, const int g, const int Z, const int m, std::string  s): entity(std::move(n)), period(p), group(g), atomicNumber(Z), atomicMass(m), symbol(std::move(s)) {
    if (atomicMass < atomicNumber)
        throw chemistryLawsException("Atom invalid: masa atomica " + std::to_string(atomicMass) + " este mai mica decat numarul atomic " + std::to_string(atomicNumber));

    if (period < 1 || period > 7)
        throw chemistryLawsException("Atom invalid: perioada trebuie sa fie un numar intre 1 si 7");

    std::cout<<"Un atom a fost construit - "<<getName()<<"\n";
    float radius;
    if (symbol=="H")
        radius=15.f;
    else radius=30.f;

    position={100.f,100.f};
    atomShape.setRadius(radius);
    atomShape.setOutlineColor(sf::Color::Black);
    atomShape.setOutlineThickness(2.f);
    atomShape.setOrigin({radius,radius});
    atomShape.setPosition(position);

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
        case 17: atomShape.setFillColor(sf::Color::Green);
            break;
        case 11: atomShape.setFillColor(sf::Color(253,169,255));
            break;
        default: atomShape.setFillColor(sf::Color(255,255,204));
    }

    int valence = atomValence();
    for (int i = 0; i < valence; ++i) {
        sf::CircleShape dot;
        dot.setRadius(3.f);
        dot.setFillColor(sf::Color::Black);
        dot.setOrigin({3.f, 3.f});
        electrons.push_back(dot);
    }
    showElectrons=false;
    availableElectrons=valence;
}
atom::atom(const atom& other): entity(other), period(other.period), group(other.group), atomicNumber(other.atomicNumber),atomicMass(other.atomicMass), symbol(other.symbol), atomShape(other.atomShape), electrons(other.electrons), showElectrons(other.showElectrons), availableElectrons(other.availableElectrons)
{
    position=other.position;
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
        position=other.position;

        electrons=other.electrons;
        showElectrons=other.showElectrons;
        availableElectrons=other.availableElectrons;
    }
    return *this;
}
atom::~atom(){std::cout<<"Un atom a fost distrus - "<<getName()<<"\n";}
std::shared_ptr<entity> atom::clone() const {return std::make_shared<atom>(*this);}

void atom::setAtomOutlineColor(const sf::Color newColor)
{
    atomShape.setOutlineColor(newColor);
}

void atom::setAtomThickness(float thickness)
{
    atomShape.setOutlineThickness(thickness);
}

void atom::onPositionChanged()
{
    atomShape.setPosition(position);
}

int atom::getAtomicMass() const {return atomicMass;}
const std::string& atom::getSymbol()const {return symbol;}
sf::FloatRect atom::getBounds() const
{
    return atomShape.getGlobalBounds();
}
sf::Vector2f atom::getAtomPosition() const
{
    return position;
}

int atom::atomValence() const
{
    if (group==1 || group==2 || group==3)
        return group;
    return 8-group;
}

void atom::move(sf::Vector2f newPosition)
{
    position=newPosition;
    atomShape.setPosition(position);
}

void atom::setShowElectrons(const bool show)
{
    showElectrons=show;
}
void atom::setAvailableElectrons(const int newCount)
{
    availableElectrons=newCount;
}

void atom::draw(sf::RenderWindow& window) const
{
    window.draw(atomShape);

    float r = atomShape.getRadius() + 8.f;
    const sf::Vector2f center = position;
    float off = 5.f;
    std::vector<sf::Vector2f> offsets;
    const int count = electrons.size();

    if (count >= 1)
        offsets.push_back({ (count >= 5 ? -off : 0), -r });
    if (count >= 5)
        offsets.push_back({ off, -r });
    if (count >= 2)
        offsets.push_back({ r, (count >= 6 ? -off : 0) });
    if (count >= 6)
        offsets.push_back({ r, off });
    if (count >= 3)
        offsets.push_back({ (count >= 7 ? -off : 0), r });
    if (count >= 7)
        offsets.push_back({ off, r });
    if (count >= 4)
        offsets.push_back({ -r, (count >= 8 ? -off : 0) });
    if (count >= 8)
        offsets.push_back({ -r, off });

    for (int i = 0; i < availableElectrons; ++i)
        if (static_cast<size_t>(i) < offsets.size())
        {
            auto& dot = const_cast<sf::CircleShape&>(electrons[i]);
            dot.setPosition(center + offsets[i]);
            if (showElectrons==true)
                window.draw(dot);
        }
}
void atom::restrictAtomToBounds(const sf::FloatRect& bounds)
{
    float minX = bounds.position.x;
    float maxX = bounds.position.x + bounds.size.x;
    float minY = bounds.position.y;
    float maxY = bounds.position.y + bounds.size.y;

    float radius = atomShape.getRadius();
    sf::Vector2f currentPosition = position;

    if (currentPosition.x - radius < minX)
        currentPosition.x = minX + radius;
    else if (currentPosition.x + radius > maxX)
        currentPosition.x = maxX - radius;

    if (currentPosition.y - radius < minY)
        currentPosition.y = minY + radius;
    else if (currentPosition.y + radius > maxY)
        currentPosition.y = maxY - radius;

    if (currentPosition != position)
        move(currentPosition);
}

std::ostream& operator<<(std::ostream& out, const atom& ATOM)
    {
        out << "Atomul " << ATOM.getName() << " cu simbolul " << ATOM.symbol << " se afla in perioada " << ATOM.period << ", grupa principala " << ATOM.group;
        out << " are numarul atomic " << ATOM.atomicNumber << " si masa atomica " << ATOM.atomicMass << "\n";
        return out;
    }

