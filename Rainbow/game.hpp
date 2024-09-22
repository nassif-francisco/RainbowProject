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

    std::map<std::string, Tile> tileAtlas;
    std::vector<std::string> tileNames;
    std::vector<std::string> brushNames;

    TextureManager texmgr;

    sf::Sprite background;
    sf::Sprite toolbar;

    void pushState(GameState* state);
    void popState();
    void changeState(GameState* state);
    GameState* peekState();

    void gameLoop();

    Game();
    ~Game();
};

#endif /* GAME_HPP */