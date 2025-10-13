#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "game.hpp";
#include "game_state_start.hpp"
#include <thread>


void separate_thread_function() {
    sf::RenderWindow window(sf::VideoMode(320, 240), "title", sf::Style::Default);
    window.setActive(true);

    sf::CircleShape shape(50.f);
    shape.setOrigin(50.f, 50.f);
    shape.setPosition((float)sf::VideoMode(320, 240).width / 2.f, (float)sf::VideoMode(320, 240).height / 2.f);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(5.f);
    shape.setOutlineColor(sf::Color::Black);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();

            }
            else if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
            {
                window.close();

            }
        }

        float dt = clock.restart().asSeconds();
        window.draw(shape);
        window.display();
    }

    if (window.isOpen())
        window.close();
}

int main()
{
    Game game;
    GameStateStart* gameStateStart = new GameStateStart(&game);
    game.pushState(gameStateStart);

    std::thread my_thread(separate_thread_function);
    game.gameLoop();
    my_thread.join();


    return 0;
}

