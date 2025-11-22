#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <SFML/Graphics.hpp>

class entity
{
    std::string name;
public:
    explicit entity(std::string);
    entity (const entity& other);
    entity& operator=(const entity& other);
    virtual ~entity();
    [[nodiscard]] virtual std::shared_ptr<entity> clone() const = 0;

    [[nodiscard]] virtual sf::FloatRect getBounds() const = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    [[nodiscard]] const std::string& getName()const;

};


#endif