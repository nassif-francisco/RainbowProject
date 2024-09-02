#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "game.hpp";
#include "game_state_start.hpp"

int main()
{
    Game game;
    GameStateStart* gameStateStart = new GameStateStart(&game);
    game.pushState(gameStateStart);
    game.gameLoop();

    return 0;
}