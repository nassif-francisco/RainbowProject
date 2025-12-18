#ifndef GAME_STATE_START_HPP
#define GAME_STATE_START_HPP

#include <SFML/Graphics.hpp>

#include "game_state.hpp"
#include "canvas.hpp"
#include "map.hpp"
#include "constants.hpp"
#include <string>
#include "mapGrid.hpp"
#include <random>


class GameStateStart : public GameState
{
private:

    sf::View view;
    sf::View guiView;

    sf::Sprite brickBrushIcon;

    int* currentBrush = nullptr;
    TileType currentPaintingGroundType = TileType::BACKGROUND;

    Tile* currentTileMaster = nullptr;
    Tile* currentTileMasterDelegate = nullptr;

    int* currentTileHovered = nullptr;
    bool LControlKeyPressed = false;
    bool SKeyPressed = false;
    bool LKeyPressed = false;
    bool TKeyPressed = false;
    bool MKeyPressed = false;
    bool DKeyPressed = false;
    bool BKeyPressed = false;
    bool FKeyPressed = false;
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
        else if (this->game->currentResolution == "_1440x900")
        {
            return RBConstants::toolbarHeightFactor_1440x900;
        }
	}

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

    sf::Vector2f GetCurrentTileMasterTopLeftCorner()
    {
        float tileXPosition = currentTileMasterDelegate->sprite.getGlobalBounds().getPosition().x;
        float tileYPosition = currentTileMasterDelegate->sprite.getGlobalBounds().getPosition().y;

        return sf::Vector2f(tileXPosition, tileYPosition);
    }
    sf::Vector2f GetCurrentTileMasterTopRightCorner()
    {
        float width = currentTileMasterDelegate->sprite.getGlobalBounds().getSize().x;
        float tileXPosition = currentTileMasterDelegate->sprite.getGlobalBounds().getPosition().x;
        float tileYPosition = currentTileMasterDelegate->sprite.getGlobalBounds().getPosition().y;
        
        return sf::Vector2f( tileXPosition + width, tileYPosition);
    }

    sf::Vector2f GetCurrentTileMasterBottomLeftCorner()
    {
        float height = currentTileMasterDelegate->sprite.getGlobalBounds().getSize().y;
        float tileXPosition = currentTileMasterDelegate->sprite.getGlobalBounds().getPosition().x;
        float tileYPosition = currentTileMasterDelegate->sprite.getGlobalBounds().getPosition().y;

        return sf::Vector2f(tileXPosition, tileYPosition + height);
    }
    sf::Vector2f GetCurrentTileMasterBottomRightCorner()
    {
        float height = currentTileMasterDelegate->sprite.getGlobalBounds().getSize().y;
        float width = currentTileMasterDelegate->sprite.getGlobalBounds().getSize().x;
        float tileXPosition = currentTileMasterDelegate->sprite.getGlobalBounds().getPosition().x;
        float tileYPosition = currentTileMasterDelegate->sprite.getGlobalBounds().getPosition().y;

        return sf::Vector2f(tileXPosition + width, tileYPosition + height);
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
    void executePaintingWithMaster(sf::Vector2f worldPos);
    void eraseTile();
    void eraseHitbox();
    //std::string getCurrentTyleID();

public:

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();
    virtual void updateObjects();

    GameStateStart(Game* game);

};

#endif /* GAME_STATE_START_HPP */