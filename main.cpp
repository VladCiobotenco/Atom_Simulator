#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include <optional>

class atom
{
    int period, group, atomicNumber, atomicMass;
    std::string name, symbol;
public:

    atom(const int p, const int g, const int Z, const int m, std::string  n, std::string  s):
    period(p), group(g), atomicNumber(Z), atomicMass(m), name(std::move(n)), symbol(std::move(s))
    {
        std::cout<<"Un atom a fost construit\n";
    }

    atom(const atom& other):
    period(other.period), group(other.group), atomicNumber(other.atomicNumber), atomicMass(other.atomicMass), name(other.name), symbol(other.symbol)
    {
        std::cout<<"Un atom a fost copiat - "<<other.name<<"\n";
    }

    atom& operator=(const atom& other)
    {
        if (this != &other)
        {
            period = other.period;
            group = other.group;
            atomicNumber = other.atomicNumber;
            atomicMass = other.atomicMass;
            name = other.name;
            symbol = other.symbol;
        }
        return *this;
    }

    ~atom(){std::cout<<"Un atom a fost distrus\n";}

    [[nodiscard]] const std::string& getName() const {return name;}

    int getAtomicMass() const {return atomicMass;}

    int atomValence() const
    {
        if (group==1 || group==2 || group==3)
            return group;
        return 8-group;
    }

    friend std::ostream& operator<<(std::ostream&, const atom&);

};
    std::ostream& operator<<(std::ostream& out, const atom& ATOM)
    {
        out<<"Atomul "<<ATOM.name<<" cu simbolul "<<ATOM.symbol<<" se afla in perioada "<<ATOM.period<<", grupa principala "<<ATOM.group;
        out<<" are numarul atomic "<<ATOM.atomicNumber<<" si masa atomica "<<ATOM.atomicMass<<"\n";
        return out;
    }


class bond
    {
        int atomIndex1, atomIndex2;
        std::string type;

    public:
        bond(int atom_index1, int atom_index2, std::string  type): atomIndex1(atom_index1), atomIndex2(atom_index2), type(std::move(type)){}

        bond(const bond& other): atomIndex1(other.atomIndex1), atomIndex2(other.atomIndex2),type(other.type){}

        bond& operator=(const bond& other)
        {
            if (this != &other)
            {
                atomIndex1 = other.atomIndex1;
                atomIndex2 = other.atomIndex2;
                type = other.type;
            }
            return *this;
        }

        ~bond(){};

        friend std::ostream& operator<<(std::ostream&, const bond&);

    };

std::ostream& operator<<(std::ostream& out, const bond& BOND)
{
    out<<"Legatura "<<BOND.type<<" intre atomul cu indexul "<<BOND.atomIndex1<<" si atomul cu indexul "<<BOND.atomIndex2<<"\n";
    return out;
}



class molecule
{
    std::string name;
    std::vector<atom>atomsList;
    std::vector<bond>bondsList;

public:
    explicit molecule(std::string  n):name(std::move(n))
    {
        std::cout<<"O molecula a fost construita\n";
    }

    molecule(const molecule& other): name(other.name), atomsList(other.atomsList), bondsList(other.bondsList){}

    molecule& operator=(const molecule& other)
    {
        if (this != &other)
        {
            name = other.name;
            atomsList = other.atomsList;
        }
        return *this;
    }

    ~molecule(){std::cout<<"O molecula a fost distrusa\n";}

    void addAtom(const atom& ATOM)
    {
        atomsList.push_back(ATOM);
    }

    void removeAtom()
    {
        atomsList.pop_back();
    }

    void addBond(const bond& BOND)
    {
        bondsList.push_back(BOND);
    }

    void removeBond()
    {
        bondsList.pop_back();
    }

    int moleculeMass()
    {
        int m=0;
        for (auto i = atomsList.begin();i<atomsList.end();++i)
            m=m+i->getAtomicMass();
        return m;
    }

    friend std::ostream& operator<<(std::ostream&, molecule&);

};

std::ostream& operator<<(std::ostream& out, molecule& MOLECULE)
{
    out<<"Molecula "<<MOLECULE.name<<" contine urmatorii atomi: ";
    std::vector<atom>::iterator i;
    for (i=MOLECULE.atomsList.begin();i<MOLECULE.atomsList.end()-1;++i)
    {
        out<<i->getName()<<", ";
    }
    out<<i->getName();
    out<<"\n";
    return out;
}



int main()
{
    atom H(1,1,1,1,"Hidrogen","H");
    atom C(2,4,6,12,"Carbon","C");
    atom N(2,5,7,14,"Azot","N");
    atom O(2,6,8,16,"Oxigen","O");

    ///Work in progress
    sf::RenderWindow mainScreen(sf::VideoMode({800, 600}), "Atom Simulator");
    while (mainScreen.isOpen())
    {
        while (const std::optional event = mainScreen.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                mainScreen.close();
        }
        mainScreen.clear(sf::Color::Cyan);
        mainScreen.display();
    }


    return 0;
}