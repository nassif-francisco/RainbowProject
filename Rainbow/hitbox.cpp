#include <SFML/Graphics.hpp>

#include "hitbox.hpp"

void Hitbox::draw(sf::RenderWindow& window, float dt)
{
    sf::RectangleShape rectangle;

    // change the size to 100x100
    rectangle.setSize({ AABB[1].x - AABB[0].x, AABB[3].y - AABB[1].y });
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor(sf::Color(250, 150, 100));
    rectangle.setPosition({ AABB[0].x, AABB[0].y });
    /* Draw the tile */

    window.draw(rectangle);
    drawVertices(window);

    return;
}

void Hitbox::update()
{

}

void Hitbox::drawVertices(sf::RenderWindow& window)
{
    for (int k = 0; k<AABB.size(); k++)
    {
        sf::CircleShape circle;
        // change the size to 100x100
        circle.setRadius({ 8.f });
        circle.setFillColor(sf::Color(150, 50, 250));
        circle.setPosition({ AABB[k].x - 8, AABB[k].y - 8 });
        /* Draw the tile */
        window.draw(circle);
    } 
}