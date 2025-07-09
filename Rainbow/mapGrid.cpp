#include <SFML/Graphics.hpp>
#include "mapGrid.hpp"


MapGrid::MapGrid(sf::Vector2i upperLeftCorner, double tileSizeX, double tileSizeY, double repFactor)
{
	UpperLeftCornerPosition = upperLeftCorner;
	TileSizeX = tileSizeX;
	TileSizeY = tileSizeY;
	RepetitionFactor = repFactor;
}


MapGrid::~MapGrid()
{
}

sf::Vector2f MapGrid::GetMousePosition(sf::RenderWindow& window)
{
	return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

sf::Vector2i MapGrid::GetWindowUpperLeftCornerPosition(sf::RenderWindow& window)
{
	return sf::Vector2i(window.getPosition().x, window.getPosition().y);
}

sf::Vector2i MapGrid::GetMapUpperLeftCornerPosition(sf::RenderWindow& window)
{
	sf::Vector2i position = GetWindowUpperLeftCornerPosition(window);

	return sf::Vector2i(position.x - RBConstants::InitialMapOffset, position.y - RBConstants::InitialMapOffset);
}

MapGrid MapGrid::CreateMapGrid(sf::RenderWindow& window)
{
	sf::Vector2i position = GetMapUpperLeftCornerPosition(window);
	MapGrid newMapGrid = MapGrid(position, 13, 13, 1000);

	return newMapGrid;
}