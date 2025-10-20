#include <iostream>
#include <string>
#include <vector>


class atom
{
    int period, group, atomicNumber, atomicMass;
    std::string name,symbol;
public:
    ///
    ///Constructor de initializare, de copiere si destructor
    ///
    atom(const int p, const int g, const int Z, const int m, const std::string& n, const std::string& s)
    {
        period=p; group=g; atomicNumber=Z; atomicMass=m; name=n; symbol=s;
        std::cout<<"Un atom a fost construit\n";
    }
    atom (const atom& other)
    {
        period=other.period; group=other.group; atomicNumber=other.atomicNumber; atomicMass=other.atomicMass;
        name=other.name; symbol=other.symbol;
        std::cout<<"Un atom a fost copiat\n";
    }
    ~atom(){std::cout<<"Un atom a fost distrus\n";}

    ///Supraincarcarea operatorului <<
    std::ostream& operator<<(std::ostream& out) const
    {
        out<<"Atomul "<<name<<", simbol "<<symbol<<", aflat in perioada "<<period<<", grupa principala "<<group<<", cu numarul atomic "<<atomicNumber<<" si numarul de masa "<<atomicMass<<"\n";
        return out;
    }

    ///Crearea de getters
    std::string getName(){return name;}
    [[nodiscard]] int getAtomicMass() const {return atomicMass;}



};

class molecule
{
    std::string name;
    std::vector<atom>a;

public:
    explicit molecule(const std::string& n){name=n; std::cout<<"O molecula a fost construita\n";}
    //copy constructor required
    ~molecule(){std::cout<<"O molecula a fost distrusa\n";}

    void addAtom(const atom& ATOM)
    {
        a.push_back(ATOM);
    }

    void removeAtom()
    {
        a.pop_back();
    }

    std::ostream& operator<<(std::ostream& out)
    {
        out<<"Molecula "<<name<<" contine urmatorii atomi: ";
        std::vector<atom>::iterator i;
        for (i=a.begin();i<a.end()-1;++i)
        {
            out<<i->getName()<<", ";
        }
        out<<i->getName();
        out<<"\n";
        return out;
    }

    int moleculeMass()
    {
        int m=0;
        for (auto i = a.begin();i<a.end();++i)
            m=m+i->getAtomicMass();
        return m;
    }

};

int main()
{
    atom H(1,1,1,1,"Hidrogen","H");
    atom C(2,4,6,12,"Carbon","C");
    atom N(2,5,7,14,"Azot","N");
    atom O(2,6,8,16,"Oxigen","O");

    molecule H2O("Water");
    H2O.addAtom(H);
    H2O.addAtom(H);
    H2O.addAtom(O);

    H2O.operator<<(std::cout);
    std::cout<<H2O.moleculeMass()<<"\n";

    H2O.removeAtom();
    H2O.removeAtom();
    H2O.removeAtom();



    return 0;
}