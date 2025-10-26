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
    bond(int, int, std::string);
    bond(const bond&);
    bond& operator=(const bond&);
    ~bond();

    friend std::ostream& operator<<(std::ostream&, const bond&);
};

std::ostream& operator<<(std::ostream& out, const bond& BOND);

#endif