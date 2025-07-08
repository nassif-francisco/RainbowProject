#include <SFML/Graphics.hpp>
#include "coordinateHandler.hpp"


MapGrid::MapGrid(sf::Vector2i upperLeftCorner, double tileSizeX, double tileSizeY, double repFactor)
{
	UpperLeftCornerPosition = upperLeftCorner;
	TileSizeX = tileSizeX;
	TileSizeY = tileSizeY;
	RepetitionFactor = repFactor;
	CurrentTilePosition = (sf::Vector2f)upperLeftCorner;
}


MapGrid::~MapGrid()
{
}