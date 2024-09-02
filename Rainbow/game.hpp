#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <SFML/Graphics.hpp>
#include "texture_manager.hpp"
#include "tile.hpp"

class GameState;

class Game
{
private:
    void loadTextures();
    void loadTiles();

public:

    std::stack<GameState*> states;

    const static int tileHeight = 8;
    const static int tileWidth = 16;

    sf::RenderWindow window;

    std::map<std::string, Tile> tileAtlas;

    TextureManager texmgr;

    sf::Sprite background;

    void pushState(GameState* state);
    void popState();
    void changeState(GameState* state);
    GameState* peekState();

    void gameLoop();

    Game();
    ~Game();
};

#endif /* GAME_HPP */