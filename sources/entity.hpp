#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>

class entity
{
    std::string name;
public:
    explicit entity(std::string);
    entity (const entity& other);
    entity& operator=(const entity& other);
    ~entity();

    const std::string& getName()const;
};


#endif