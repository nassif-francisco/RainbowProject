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
	std::string backgroundPath = RBConstants::CommonMediaEnvironmentPath + "background" + currentResolution + ".png";
	std::string toolbarPath = RBConstants::CommonMediaEnvironmentPath + "toolbar" + currentResolution + ".png";
	std::string boardPath = RBConstants::CommonMediaEnvironmentPath + "board" + currentResolution + ".png";

    texmgr.loadTexture("background", backgroundPath);
    texmgr.loadTexture("toolbar", toolbarPath);
    texmgr.loadTexture("board", boardPath);
    /*texmgr.loadTexture("flower", "media/Packs/Forest/flower.png");
    texmgr.loadTexture("flowerButton", "media/Packs/Forest/Grid1Button.png");*/



    //begin toolbar region
    std::string directoryPath = RBConstants::CommonMediaEnvironmentPacksPath + "Toolbar"; // Replace with your directory path

    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                std::string numberOfFrames = "";
                
                fs::path filePath = entry.path();
                std::string filename = filePath.filename().string();

                for (size_t pos = 0; (pos = filename.find('_', pos)) != std::string::npos; ++pos) {
                    if (pos + 1 < filename.size() && std::isdigit(filename[pos + 1])) 
                    { // Check if next character is a digit
                        numberOfFrames = filename[pos + 1];
                    }
                }

                std::string buttonString = "Button";

                std::string finalPath = filePath.string();
                std::string finalPath2 = finalPath.replace(finalPath.find("\\"), 1, "/");

                // Check if the filename does not end with "Button"
                if (filename.find(buttonString) == std::string::npos) {
                    std::string tileName = filename.replace(filename.find(".png"), 4, "");
                    if (!numberOfFrames.empty())
                    {
                        tileSprites[filename] = std::stoi(numberOfFrames);
                    }
                    else
                    {
                        tileNames.push_back(tileName);
                    }

                    texmgr.loadTexture(tileName, finalPath2);
                }
                else
                {
                    std::string tileName = filename.replace(filename.find(".png"), 4, "");
                    brushNames.push_back(tileName);
                    texmgr.loadBrushTexture(tileName, finalPath2);
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
    //end of toolbar region

    //end of mainMenu region
    directoryPath = RBConstants::CommonMediaEnvironmentPacksPath + "MainMenu"; // Replace with your directory path

    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                std::string numberOfFrames = "";

                fs::path filePath = entry.path();
                std::string filename = filePath.filename().string();

                std::string actionName = filename.replace(filename.find(".png"), 4, "");

                std::string finalPath = filePath.string();
                std::string finalPath2 = finalPath.replace(finalPath.find("\\"), 1, "/");
                buttonNames.push_back(actionName);
                texmgr.loadTexture(actionName, finalPath2);
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    //end of mainMenu region





}

void Game::pushState(GameState* state)
{
    this->states.push(state);

    return;
}

void Game::popState()
{
    //delete this->states.top();
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
    
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    this->window.create(sf::VideoMode(desktop.width, desktop.height - RBConstants::toolbarHeight), "app.exe", sf::Style::Default);
    std::string currentResolution = "_" + std::to_string(desktop.width) + "x" + std::to_string(desktop.height);

    this->currentResolution = currentResolution;
    
    this->loadTextures();
    this->loadTiles();
   
    //local tweat to set window in corner
    this->window.setPosition(sf::Vector2i(-10, -5));
    this->window.setFramerateLimit(60);

    this->background.setTexture(this->texmgr.getRef("background"));
    this->toolbar.setTexture(this->texmgr.getRef("toolbar"));
    this->mainMenu.setTexture(this->texmgr.getRef("toolbar"));
    this->board.setTexture(this->texmgr.getRef("board"));
}

void Game::loadTiles()
{
    Animation staticAnim(0, 0, 1.0f);

    for (const std::string& tile : tileNames) {
        if (tile == "C:\\Users\\franc\\Downloads\\newTileSet.png")
        {
            int a = 0;
        }
        tileAtlas[tile] =
            Tile(tile, 25, 1, texmgr.getRef(tile),
                { staticAnim },
                TileType::BACKGROUND, 100, 0, 1);
    }
    sf::Texture textu = texmgr.getRef("TestTileSprite_4");
    sf::Vector2u size = textu.getSize();

    for (const auto& pair : tileSprites) {
        tileAtlas[pair.first] =
            Tile(pair.first, pair.second, texmgr.getRef(pair.first),
                { Animation(0, pair.second - 1, DEFAULT_FRAME_RATE) },//this 0.5f is an arbitrary velocity set for now
                TileType::BACKGROUND, 0, 0, 1, true);
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