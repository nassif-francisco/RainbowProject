#ifndef CANVASOBJECT_HPP
#define CANVASOBJECT_HPP
#include "animation_handler.hpp"

class CanvasObject
{

private:
public:

	sf::Sprite sprite;
	void draw(sf::RenderWindow& window, float dt);
	AnimationHandler animHandler;
	int tileVariant;

};




#endif /* CANVASOBJECT_HPP */