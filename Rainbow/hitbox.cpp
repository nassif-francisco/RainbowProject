#include <SFML/Graphics.hpp>

#include "hitbox.hpp"
#include "constants.hpp"

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
    Rectangle = rectangle;

    window.draw(rectangle);
    drawVerticesAndHandle(window);

    return;
}

void Hitbox::update()
{

}

void Hitbox::drawVerticesAndHandle(sf::RenderWindow& window)
{
    for (int k = 0; k<AABB.size(); k++)
    {
        sf::CircleShape circle;
        // change the size to 100x100
        circle.setRadius({ RBConstants::VerticesSize });
        circle.setFillColor(sf::Color(150, 50, 250));
        circle.setPosition({ AABB[k].x - RBConstants::VerticesSize, AABB[k].y - RBConstants::VerticesSize });
        /* Draw the tile */
        VertexHandles.push_back(circle);
        window.draw(circle);
    } 

    //draw handle
    sf::CircleShape circle;
    // change the size to 100x100
    circle.setRadius({ RBConstants::VerticesSize });
    circle.setFillColor(sf::Color(76, 175, 80));
    float handleCenterX = ((AABB[0].x - RBConstants::VerticesSize) + (AABB[1].x - RBConstants::VerticesSize)) / 2;
    float handleCenterY = ((AABB[1].y - RBConstants::VerticesSize) + (AABB[3].y - RBConstants::VerticesSize)) / 2;
    circle.setPosition({ handleCenterX, handleCenterY });
    /* Draw the tile */
    MainHandle = circle;
    window.draw(circle);

}