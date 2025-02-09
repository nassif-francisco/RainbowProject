#include <SFML/Graphics.hpp>

#include "hitbox.hpp"
#include "constants.hpp"

void Hitbox::draw(sf::RenderWindow& window, float dt)
{
    if (Rectangle == nullptr)
    {
        Rectangle = new sf::RectangleShape();

        Rectangle->setSize({ AABB[1].x - AABB[0].x, AABB[3].y - AABB[1].y });
        Rectangle->setFillColor(sf::Color::Transparent);
        Rectangle->setOutlineThickness(2.f);
        Rectangle->setOutlineColor(sf::Color(250, 150, 100));
        Rectangle->setPosition({ AABB[0].x, AABB[0].y });
    }
   

    window.draw(*Rectangle);
    drawVerticesAndHandle(window);

    return;
}

void Hitbox::update()
{

}

void Hitbox::drawVerticesAndHandle(sf::RenderWindow& window)
{
    if (VertexHandles.size() == 0)
    {
        for (int k = 0; k < AABB.size(); k++)
        {
            sf::CircleShape circle;
            // change the size to 100x100
            circle.setRadius({ RBConstants::VerticesSize });
            circle.setFillColor(sf::Color(150, 50, 250));
            circle.setPosition({ AABB[k].x - RBConstants::VerticesSize, AABB[k].y - RBConstants::VerticesSize });
            /* Draw the tile */
            VertexHandles.push_back(circle);
            
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
    }

    for (const auto& circle : VertexHandles)
    {
        window.draw(circle);
    }
    
    window.draw(MainHandle);

}