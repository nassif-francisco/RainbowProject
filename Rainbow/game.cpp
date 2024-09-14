#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "game.hpp"
#include "game_state.hpp"
#include "texture_manager.hpp"
#include "animation_handler.hpp"

#include <iostream>
#include <filesystem>
#include <string>


void Game::loadTextures()
{
    //texmgr.loadTexture("grass", "media/grass.png");
    //texmgr.loadTexture("forest", "media/Packs/Forest/Grid1.png");
    //texmgr.loadTexture("water", "media/water.png");
    //texmgr.loadTexture("residential", "media/residential.png");
    //texmgr.loadTexture("commercial", "media/commercial.png");
    //texmgr.loadTexture("industrial", "media/industrial.png");
    //texmgr.loadTexture("road", "media/road.png");
    texmgr.loadTexture("background", "media/background.png");
    texmgr.loadTexture("toolbar", "media/toolbar.png");
    /*texmgr.loadTexture("flower", "media/Packs/Forest/flower.png");
    texmgr.loadTexture("flowerButton", "media/Packs/Forest/Grid1Button.png");*/

    std::string directoryPath = "media/Packs/Forest"; // Replace with your directory path

    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                fs::path filePath = entry.path();
                std::string filename = filePath.filename().string();
                std::string buttonString = "Button";

                std::string finalPath = filePath.string();
                std::string finalPath2 = finalPath.replace(finalPath.find("\\"), 1, "/");

                // Check if the filename ends with "Button"
                if (filename.find(buttonString) == std::string::npos) {
                    std::string tileName = filename.replace(filename.find(".png"), 4, "");
                    tileNames.push_back(tileName);
                    texmgr.loadTexture(tileName, finalPath2);
                }
                else
                {
                    std::string tileName = filename.replace(filename.find(".png"), 4, "");
                    brushNames.push_back(tileName);
                    texmgr.loadTexture(tileName, finalPath2);
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}

void Game::pushState(GameState* state)
{
    this->states.push(state);

    return;
}

void Game::popState()
{
    delete this->states.top();
    this->states.pop();

    return;
}

void Game::changeState(GameState* state)
{
    if (!this->states.empty())
        popState();
    pushState(state);

    return;
}

GameState* Game::peekState()
{
    if (this->states.empty()) return nullptr;
    return this->states.top();
}

void Game::gameLoop()
{
    sf::Clock clock;

    while (this->window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        if (peekState() == nullptr) continue;
        peekState()->handleInput();
        peekState()->update(dt);
        this->window.clear(sf::Color::Black);
        peekState()->draw(dt);
        this->window.display();
    }
}

Game::Game()
{
    this->loadTextures();
    this->loadTiles();
    
    this->window.create(sf::VideoMode(892, 658), "City Builder");
    this->window.setFramerateLimit(60);

    this->background.setTexture(this->texmgr.getRef("background"));
    this->toolbar.setTexture(this->texmgr.getRef("toolbar"));
}

void Game::loadTiles()
{
    Animation staticAnim(0, 0, 1.0f);

    for (const std::string& tile : tileNames) {
        tileAtlas[tile] =
            Tile(25, 1, texmgr.getRef(tile),
                { staticAnim },
                TileType::FOREST, 100, 0, 1);
    }


   /* this->tileAtlas["grass"] =
        Tile(this->tileHeight, 1, texmgr.getRef("grass"),
            { staticAnim },
            TileType::GRASS, 50, 0, 1);
    tileAtlas["forest"] =
        Tile(25, 1, texmgr.getRef("forest"),
            { staticAnim },
            TileType::FOREST, 100, 0, 1);
    tileAtlas["water"] =
        Tile(this->tileHeight, 1, texmgr.getRef("water"),
            { Animation(0, 3, 0.5f),
            Animation(0, 3, 0.5f),
            Animation(0, 3, 0.5f) },
            TileType::WATER, 0, 0, 1);
    tileAtlas["residential"] =
        Tile(this->tileHeight, 2, texmgr.getRef("residential"),
            { staticAnim, staticAnim, staticAnim,
            staticAnim, staticAnim, staticAnim },
            TileType::RESIDENTIAL, 300, 50, 6);
    tileAtlas["commercial"] =
        Tile(this->tileHeight, 2, texmgr.getRef("commercial"),
            { staticAnim, staticAnim, staticAnim, staticAnim },
            TileType::COMMERCIAL, 300, 50, 4);
    tileAtlas["industrial"] =
        Tile(this->tileHeight, 2, texmgr.getRef("industrial"),
            { staticAnim, staticAnim, staticAnim,
            staticAnim },
            TileType::INDUSTRIAL, 300, 50, 4);
    tileAtlas["road"] =
        Tile(this->tileHeight, 1, texmgr.getRef("road"),
            { staticAnim, staticAnim, staticAnim,
            staticAnim, staticAnim, staticAnim,
            staticAnim, staticAnim, staticAnim,
            staticAnim, staticAnim },
            TileType::ROAD, 100, 0, 1);*/

    return;
}

Game::~Game()
{
    while (!this->states.empty()) popState();
}