#include "bond.hpp"
#include <utility>

bond::bond(int atomIndex1, int atomIndex2, std::string  type): atomIndex1(atomIndex1), atomIndex2(atomIndex2), type(std::move(type)){}
bond::bond(const bond& other): atomIndex1(other.atomIndex1), atomIndex2(other.atomIndex2),type(other.type){}
bond& bond::operator=(const bond& other)
{
    if (this != &other)
    {
        atomIndex1 = other.atomIndex1;
        atomIndex2 = other.atomIndex2;
        type = other.type;
    }
    return *this;
}
bond::~bond(){};

std::ostream& operator<<(std::ostream& out, const bond& BOND)
{
    out<<"Legatura "<<BOND.type<<" intre atomul cu indexul "<<BOND.atomIndex1<<" si atomul cu indexul "<<BOND.atomIndex2<<"\n";
    return out;
}