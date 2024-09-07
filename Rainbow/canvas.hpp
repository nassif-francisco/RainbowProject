#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <SFML/Graphics.hpp>
#include "CanvasObject.hpp"
#include <vector>
#include "game.hpp"

class Canvas
{


private:

public:
	std::vector<CanvasObject> canvasObjects;
	void draw(sf::RenderWindow& window, float dt);
	Game* game;

};


#endif /* CANVAS_HPP */