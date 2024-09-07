#ifndef GAME_STATE_START_HPP
#define GAME_STATE_START_HPP

#include <SFML/Graphics.hpp>

#include "game_state.hpp"
#include "canvas.hpp"
#include "map.hpp"

class GameStateStart : public GameState
{
private:

    sf::View view;
    sf::View guiView;

    sf::Sprite brickBrushIcon;
    Canvas canvas;
    Map map;
    void loadgame();

public:

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    GameStateStart(Game* game);

};

#endif /* GAME_STATE_START_HPP */