#ifndef OOP_CHEMICAL_DATABASE_HPP
#define OOP_CHEMICAL_DATABASE_HPP

#include <map>
#include <string>

class chemical_database
{
    std::map<std::string, std::string> database;
public:
    void loadIntoDatabase(const std::string&);
    std::string searchIntoDatabase(const std::string&);
    friend std::ostream& operator<<(std::ostream&, const chemical_database&);
};


#endif