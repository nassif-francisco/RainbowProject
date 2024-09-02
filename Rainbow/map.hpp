#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <vector>
#include "game.hpp"

#include "tile.hpp"

class Map
{
private:

    void depthfirstsearch(std::vector<TileType>& whitelist,
        sf::Vector2i pos, int label, int type);

public:

    unsigned int width;
    unsigned int height;

    std::vector<Tile> tiles;

    /* Resource map */
    std::vector<int> resources;

    unsigned int tileHeight;

    Game* game;

    unsigned int numSelected;

    unsigned int numRegions[1];

    /* Load map from disk */
    void load(const std::string& filename, unsigned int width, unsigned int height,
        std::map<std::string, Tile>& tileAtlas);

    /* Save map to disk */
    void save(const std::string& filename);

    /* Draw the map */
    void draw(sf::RenderWindow& window, float dt);

    void GetDiamondPointZero();

    /* Checks if one position in the map is connected to another by
     * only traversing tiles in the whitelist */
    void findConnectedRegions(std::vector<TileType> whitelist, int type);

    /* Update the direction of directional tiles so that they face the correct
     * way. Used to orient roads, pylons, rivers etc */
    void updateDirection(TileType tileType);

    /* Blank map constructor */
    Map()
    {
        this->tileHeight = 8;
        this->width = 0;
        this->height = 0;
        this->numRegions[0] = 1;
    }
    /* Load map from file constructor */
    Map(const std::string& filename, unsigned int width, unsigned int height,
        std::map<std::string, Tile>& tileAtlas, Game* game)
    {
        this->tileHeight = 8;
        this->game = game;
        load(filename, width, height, tileAtlas);
    }
};

#endif /* MAP_HPP */