#include "entity.hpp"

#include <utility>

entity::entity(std::string entityName):name(std::move(entityName)){}

entity::entity(const entity& other): name(other.name){}

entity& entity::operator=(const entity& other)
{
    if (this!=&other)
        this->name=other.name;
    return *this;
}

entity::~entity()= default;

const std::string& entity::getName() const
{
    return name;
}




