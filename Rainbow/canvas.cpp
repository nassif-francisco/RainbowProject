#include <SFML/Graphics.hpp>
#include "canvas.hpp"


void Canvas::draw(sf::RenderWindow& window, float dt)
{
    sf::Vector2f size = sf::Vector2f(this->game->window.getSize());
    sf::Vector2f pos = sf::Vector2f(this->game->window.getPosition());
    sf::Vector2f diamondNodeZero = sf::Vector2f(pos.x, pos.y);
    diamondNodeZero.x = 960;
    diamondNodeZero.y = 240;


    pos = diamondNodeZero;
    pos.x = 10;
    pos.y = 60;
    int i = 0;
    int k = 0;
    for (int y = 0; y < 64; ++y)
    {
        pos.x = diamondNodeZero.x - (k * 8);
        pos.y = diamondNodeZero.y + (y * 4);
        for (int i = 0; i < canvasObjects.size(); ++i)
        {
            /* Set the position of the tile in the 2d world */
            //sf::Vector2f pos;
            //pos.x = (x - y) * this->tileSize + this->width * this->tileSize;
            //pos.y = (x + y) * this->tileSize * 0.5;
            pos.x += 8;
            pos.y += 4;
            /*std::cout << "station: " << y * this->width + x << std::endl;
            std::cout << "postition.x: " << pos.x + x << std::endl;
            std::cout << "postition.y: " << pos.y + x << std::endl;*/
            sf::Vector2f worldPos = this->game->window.mapPixelToCoords(sf::Vector2i(pos));
            sf::Vector2f worldPos2 = sf::Vector2f(this->game->window.mapCoordsToPixel(pos));


            /* Draw the tile */
            this->canvasObjects[i].draw(window, dt);
            i++;
        }
        k++;
    }
    return;
}