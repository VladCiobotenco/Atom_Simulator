#ifndef BOND_HPP
#define BOND_HPP
#include <iostream>
#include <string>
#include <utility>
#include <SFML/Graphics.hpp>

class bond
{
    int atomIndex1, atomIndex2;
    std::string type;
    sf::VertexArray bond_line;

public:
    bond(int atom_index1, int atom_index2, std::string  type);
    bond(const bond& other);
    bond& operator=(const bond& other);
    ~bond();
    friend std::ostream& operator<<(std::ostream&, const bond&);
};

std::ostream& operator<<(std::ostream& out, const bond& BOND);

#endif