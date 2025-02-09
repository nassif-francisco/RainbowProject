#ifndef GAME_STATE_START_HPP
#define GAME_STATE_START_HPP

#include <SFML/Graphics.hpp>

#include "game_state.hpp"
#include "canvas.hpp"
#include "map.hpp"
#include "constants.hpp"
#include <string>

class GameStateStart : public GameState
{
private:

    sf::View view;
    sf::View guiView;

    sf::Sprite brickBrushIcon;

    int* currentBrush = nullptr;
    int* currentTileHovered = nullptr;
    int* currentHitboxHovered = nullptr;
    int* currentVertexHandleHovered = nullptr;
    int* currentMainHandleHovered = nullptr;
    std::vector<sf::Sprite> Row1Brushes;
    std::vector<sf::Sprite> Row2Brushes;

    RBActionState  actionState;
    sf::Vector2i panningAnchor;
    float zoomLevel;
    std::string currentTyleID;
    float toolbarMinY;

    Canvas canvas;
    Map map;
    void loadgame();
    void assembleToolbar(Game* game, sf::Vector2f pos, sf::Vector2f size);
    void setCurrentTyleID(sf::Vector2f position);
    bool checkIfMousePositionIsOnTile(sf::Vector2f position);
    bool checkIfMouseClickIsOnTile(sf::Vector2f position);
    bool checkIfMouseClickIsOnVertexHandle(sf::Vector2f position);
    bool checkIfMouseClickIsOnMainHandle(sf::Vector2f position);
    //std::string getCurrentTyleID();

public:

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    GameStateStart(Game* game);

};

#endif /* GAME_STATE_START_HPP */