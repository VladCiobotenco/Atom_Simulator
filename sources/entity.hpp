#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <SFML/Graphics.hpp>

class entity
{
    std::string name;
protected:
    sf::Vector2f position;
    virtual void onPositionChanged(){}

public:
    explicit entity(std::string);
    entity (const entity& other);
    entity& operator=(const entity& other);
    virtual ~entity();
    [[nodiscard]] virtual std::shared_ptr<entity> clone() const = 0;

    void setPosition(const sf::Vector2f&);
    [[nodiscard]] virtual sf::FloatRect getBounds() const = 0;
    [[nodiscard]] const std::string& getName()const;                                        // cu toate ca este stupid acest get, am decis sa il pastrez pentru a-mi aminti mereu ca exista variable private

    virtual void draw(sf::RenderWindow& window) const = 0;
};


#endif