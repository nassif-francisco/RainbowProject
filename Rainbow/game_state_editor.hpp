#ifndef GAME_STATE_EDITOR_HPP
#define GAME_STATE_EDITOR_HPP

#include <SFML/Graphics.hpp>

#include "game_state.hpp"
#include "map.hpp"


enum class ActionState { NONE, PANNING };

class GameStateEditor : public GameState
{
private:

    sf::View gameView;
    ActionState actionState;
    sf::Vector2i panningAnchor;

    Map map;
    float zoomLevel;
    void loadgame();

public:

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    GameStateEditor(Game* game);
};

#endif /* GAME_STATE_EDITOR_HPP */