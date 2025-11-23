#ifndef GAME_STATE_TILE_SPLITTER_HPP
#define GAME_STATE_TILE_SPLITTER_HPP

#include <SFML/Graphics.hpp>

#include "game_state.hpp"
#include "canvas.hpp"
#include "map.hpp"
#include "constants.hpp"
#include <string>
#include "mapGrid.hpp"
#include <random>


class GameStateTileSplitter : public GameState
{
private:

    sf::View view;
    sf::View guiView;

    int* currentTileHovered = nullptr;
    bool LControlKeyPressed = false;
    bool SKeyPressed = false;
    bool OKeyPressed = false;
    int* currentHitboxHovered = nullptr;
    int* currentVertexHandleHovered = nullptr;
    int* currentMainHandleHovered = nullptr;

    float XCornerTileSet;
    float YCornerTileSet;
    string tileSetFileName;

    std::vector<sf::Sprite> MainToolbarButtons;

    RBActionState  actionState;
    sf::Vector2i panningAnchor;
    float zoomLevel;

    float toolbarMinY;
    float mainMenuMinY;

    const float GetCurrentToolbarHeightFactor()
    {
        if (this->game->currentResolution == "_1920x1080")
        {
            return RBConstants::toolbarHeightFactor_1920x1080;
        }
        else if (this->game->currentResolution == "_2560x1600")
        {
            return RBConstants::toolbarHeightFactor_2560x1600;
        }
    }

    Map map;
    void loadgame();
    void insertTileSetInScreen(string fileName);
    void insertDefaultHitboxesForSprites(string fileNam, float spritewidth, float spriteheighte);
    void createTilesUsingHitboxes(string fileName);
    void assembleToolbar(Game* game, sf::Vector2f pos, sf::Vector2f size);
    void assembleMainMenu(Game* game, sf::Vector2f pos, sf::Vector2f size);
    void assembleBoardCollection(Game* game, MapGrid& mapGrid);
    void setCurrentTyleID(sf::Vector2f position);
    void setCurrentTyleType(sf::Vector2f position);
    bool checkIfMousePositionIsOnTile(sf::Vector2f position);
    bool checkIfMouseClickIsOnTile(sf::Vector2f position);
    bool checkIfMouseClickIsOnVertexHandle(sf::Vector2f position);
    bool checkIfMouseClickIsOnMainHandle(sf::Vector2f position);
    void drawBoardCollection(Game* game);
    //std::string getCurrentTyleID();
    std::string generateRandomString(size_t length = 6)
    {
        static const std::string chars =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        // Use a random device and Mersenne Twister for high-quality randomness
        static std::random_device rd;
        static std::mt19937 generator(rd());
        std::uniform_int_distribution<> dist(0, static_cast<int>(chars.size() - 1));

        std::string result;
        result.reserve(length);

        for (size_t i = 0; i < length; ++i)
            result += chars[dist(generator)];

        return result;
    }

public:

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();
    virtual void updateObjects();

    GameStateTileSplitter(Game* game);

};

#endif /* GAME_STATE_TILE_SPLITTER_HPP */