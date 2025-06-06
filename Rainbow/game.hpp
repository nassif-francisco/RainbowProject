#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <SFML/Graphics.hpp>
#include "texture_manager.hpp"
#include "tile.hpp"

#include <iostream>
#include <filesystem>
#include <string>
#include "constants.hpp"

namespace fs = std::filesystem;


class GameState;

class Game
{
private:
    void loadTextures();
    void loadTiles();

    void processFile(const fs::path& filePath) {
        // Replace this with your actual processing code
        std::cout << "Processing file: " << filePath << std::endl;
    }

public:

    std::stack<GameState*> states;

    const static int tileHeight = 8;
    const static int tileWidth = 16;

    sf::RenderWindow window;

	std::string currentResolution;

    std::map<std::string, Tile> tileAtlas;
    std::vector<std::string> tileNames;
    std::map<std::string, int> tileSprites;
    std::vector<std::string> brushNames;

    std::vector<std::string> buttonNames;

    TextureManager texmgr;

    sf::Sprite background;
    sf::Sprite toolbar;
    sf::Sprite mainMenu;
    sf::Sprite board;

    void pushState(GameState* state);
    void popState();
    void changeState(GameState* state);
    GameState* peekState();

    void gameLoop();

    Game();
    ~Game();
};

#endif /* GAME_HPP */