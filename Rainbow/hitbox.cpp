#include <SFML/Graphics.hpp>

#include "hitbox.hpp"

void Hitbox::draw(sf::RenderWindow& window, float dt)
{
    sf::RectangleShape rectangle;

    // change the size to 100x100
    rectangle.setSize({ 100.f, 100.f });
    rectangle.setFillColor(sf::Color(150, 50, 250));
    rectangle.setPosition({ AABB[0].x, AABB[0].y });
    /* Draw the tile */
    window.draw(rectangle);

    return;
}

void Hitbox::update()
{

}