#ifndef BOND_HPP
#define BOND_HPP
#include <string>
#include <SFML/Graphics.hpp>

class bond
{
    int atomIndex1, atomIndex2;
    std::string type;
    sf::VertexArray bondLine;

public:
    bond(int, int, std::string,const sf::Vector2f&, const sf::Vector2f&);
    bond(const bond&);
    bond& operator=(const bond&);
    ~bond();

    int getAtomIndex1()const;
    int getAtomIndex2()const;

    void draw(sf::RenderWindow&) const;
    void updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2);

    friend std::ostream& operator<<(std::ostream&, const bond&);
};

std::ostream& operator<<(std::ostream& out, const bond& BOND);

#endif