#ifndef GAME_STATE_START_HPP
#define GAME_STATE_START_HPP

#include <SFML/Graphics.hpp>

#include "game_state.hpp"
#include "canvas.hpp"
#include "map.hpp"
#include "constants.hpp"
#include <string>
#include "mapGrid.hpp"


class GameStateStart : public GameState
{
private:

    sf::View view;
    sf::View guiView;

    sf::Sprite brickBrushIcon;

    int* currentBrush = nullptr;
    TileType currentPaintingGroundType = TileType::BACKGROUND;
    int* currentTileHovered = nullptr;
    bool LControlKeyPressed = false;
    bool SKeyPressed = false;
    bool LKeyPressed = false;
    bool TKeyPressed = false;
    int* currentHitboxHovered = nullptr;
    int* currentVertexHandleHovered = nullptr;
    int* currentMainHandleHovered = nullptr;
    std::vector<sf::Sprite> Row1Brushes;
    std::vector<sf::Sprite> Row2Brushes;

    std::vector<sf::Sprite> MainToolbarButtons;

    RBActionState  actionState;
    sf::Vector2i panningAnchor;
    float zoomLevel;
    std::string currentTyleID;
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

    Canvas canvas;
    Map map;
    void loadTileSplitter();
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

public:

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();
    virtual void updateObjects();

    GameStateStart(Game* game);

};

#endif /* GAME_STATE_START_HPP */