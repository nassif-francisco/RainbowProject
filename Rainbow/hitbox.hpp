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
	int* initialHoveredHandle = nullptr;

	//Constructors
	Hitbox()
	{

	}
	Hitbox(vector<sf::Vector2f> AABBPoints, int* hoveredHandle = nullptr)
	{
		AABB = AABBPoints;
		initialHoveredHandle = hoveredHandle;
	}

	void UpdatePoints(sf::Vector2f updatedCoordinates, AABBPoint point)
	{
		switch (point)
		{
		case AABBPoint::P1:
			AABB[3].x = updatedCoordinates.x; //P4.x = P1.X
			AABB[1].y = updatedCoordinates.y;//P2.y = P1.y
			break;
		case AABBPoint::P2:
			AABB[2].x = updatedCoordinates.x; //P3.x = P2.X
			AABB[0].y = updatedCoordinates.y;//P1.y = P2.y
			break;
		case AABBPoint::P3:
			AABB[1].x = updatedCoordinates.x; //P2.x = P3.X
			AABB[3].y = updatedCoordinates.y;//P4.y = P3.y
			break;
		case AABBPoint::P4:
			AABB[0].x = updatedCoordinates.x; //P1.x = P4.X
			AABB[2].y = updatedCoordinates.y;//P3.y = P4.y
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

	void updateRectangle();

	private : 

};

#endif // ! HITBOX_HPP
