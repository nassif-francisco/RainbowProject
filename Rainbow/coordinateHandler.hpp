#ifndef COORDINATEHANDLER_HPP
#define COORDINATEHANDLER_HPP

#include <stack>
#include <SFML/Graphics.hpp>
#include "texture_manager.hpp"
#include "tile.hpp"

#include <iostream>
#include <filesystem>
#include <string>
#include "constants.hpp"

namespace fs = std::filesystem;


class MapGrid
{
public:
	MapGrid(sf::Vector2i upperLeftCorner, double tileSizeX, double tileSizeY, double repFactor);
	~MapGrid();
	sf::Vector2i UpperLeftCornerPosition;
	sf::Vector2i LowerRightCornerPosition;
	double TileSizeX;
	double TileSizeY;
	double RepetitionFactor;
	sf::Vector2f CurrentTilePosition;

private:

};


static class CoordinateHandler
{

public:

	sf::Vector2f static GetMousePosition(sf::RenderWindow& window)
	{
		return window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}

	sf::Vector2i static GetWindowUpperLeftCornerPosition(sf::RenderWindow& window)
	{
		return sf::Vector2i(window.getPosition().x, window.getPosition().y);
	}

	sf::Vector2i static GetMapUpperLeftCornerPosition(sf::RenderWindow& window)
	{
		sf::Vector2i position = GetWindowUpperLeftCornerPosition(window);
		
		return sf::Vector2i(position.x - RBConstants::InitialMapOffset, position.y - RBConstants::InitialMapOffset);
	}

	MapGrid static GetMapGrid(sf::RenderWindow& window)
	{
		sf::Vector2i position = GetMapUpperLeftCornerPosition(window);
		MapGrid newMapGrid = MapGrid(position, 13, 13, 1000);

		return newMapGrid;
	}
};

#endif /* COORDINATEHANDLER_HPP */