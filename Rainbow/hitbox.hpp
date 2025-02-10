#ifndef  HITBOX_HPP
#define HITBOX_HPP
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

enum class AABBPoint { P1, P2, P3, P4};
class Hitbox
{
	public:

	vector<sf::Vector2f> AABB;
	sf::RectangleShape* Rectangle = nullptr;
	vector<sf::CircleShape> VertexHandles;
	sf::CircleShape MainHandle;

	//Constructors
	Hitbox()
	{

	}
	Hitbox(vector<sf::Vector2f> AABBPoints)
	{
		AABB = AABBPoints;
	}

	void UpdatePoints(sf::Vector2f updatedCoordinates, AABBPoint point)
	{
		switch (point)
		{
		case AABBPoint::P1:
			AABB[0] = updatedCoordinates;
			break;
		case AABBPoint::P2:
			AABB[1] = updatedCoordinates;
			break;
		case AABBPoint::P3:
			AABB[2] = updatedCoordinates;
			break;
		case AABBPoint::P4:
			AABB[3] = updatedCoordinates;
			break;
		default:
			break;
		}
	}

	void draw(sf::RenderWindow& window, float dt);

	void update();

	void drawVerticesAndHandle(sf::RenderWindow& window);

	void initializeVerticesAndMainHandle();

	void initializeRectangle();

	private : 

};

#endif // ! HITBOX_HPP
