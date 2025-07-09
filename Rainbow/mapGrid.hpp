#ifndef MAPGRID_HPP
#define MAPGRID_HPP

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

	static sf::Vector2f GetMousePosition(sf::RenderWindow& window);
	static sf::Vector2i GetWindowUpperLeftCornerPosition(sf::RenderWindow& window);
	static sf::Vector2i GetMapUpperLeftCornerPosition(sf::RenderWindow& window);
	static MapGrid CreateMapGrid(sf::RenderWindow& window);

private:

};

#endif /* COORDINATEHANDLER_HPP */