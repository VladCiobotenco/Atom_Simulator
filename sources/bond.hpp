#ifndef BOND_HPP
#define BOND_HPP
#include <string>
#include <SFML/Graphics.hpp>

#include "entity.hpp"

class bond: public entity
{
protected:
    int atomIndex1, atomIndex2;

    virtual void onDraw(sf::RenderWindow& window) const = 0;

public:
    bond(std::string, int, int);
    bond(const bond&);
    bond& operator=(const bond&);
    ~bond() override;
    [[nodiscard]] std::shared_ptr<entity> clone() const override = 0;

    int getAtomIndex1()const;
    int getAtomIndex2()const;

    [[nodiscard]] std::pair<float, float> calculateGeometry(const sf::Vector2f&, const sf::Vector2f&)const;
    void draw(sf::RenderWindow&) const override;
    virtual void updatePosition(const sf::Vector2f& pos1, const sf::Vector2f& pos2) = 0;
    [[nodiscard]] sf::FloatRect getBounds() const override = 0;
    [[nodiscard]] virtual int getOrder() const = 0;

    friend std::ostream& operator<<(std::ostream&, const bond&);
};

std::ostream& operator<<(std::ostream&, const bond&);

#endif