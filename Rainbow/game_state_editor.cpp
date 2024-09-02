#include <SFML/Graphics.hpp>

#include "game_state.hpp"
#include "game_state_editor.hpp"
#include <iostream>
#include "game_state_start.hpp"

void GameStateEditor::draw(const float dt)
{
    this->game->window.clear(sf::Color::Black);

    //this->game->window.setView(this->guiView);
    this->game->window.draw(this->game->background);

    this->game->window.setView(this->gameView);
    this->map.draw(this->game->window, dt);

    return;
}

void GameStateEditor::update(const float dt)
{
    return;
}

void GameStateEditor::handleInput()
{
    sf::Event event;

    while (this->game->window.pollEvent(event))
    {
        switch (event.type)
        {
            /* Close the window */
        case sf::Event::Closed:
        {
            this->game->window.close();
            break;
        }
        //case sf::Event::MouseButtonPressed:
        //{
        //    // get global mouse position
        //    sf::Vector2i position = sf::Mouse::getPosition();
        //    sf::Vector2i positionWindow = this->game->window.getPosition();

        //    // set mouse position relative to a window
        //    //sf::Mouse::setPosition(sf::Vector2i(100, 200), game->window);

        //    std::cout << "postition.x: " << position.x << std::endl;
        //    std::cout << "position.y: " << position.y << std::endl;

        //    sf::Vector2i pixelPos = sf::Mouse::getPosition(this->game->window);

        //    // convert it to world coordinates
        //    sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

        //    std::cout << "worldPos.x: " << worldPos.x << std::endl;
        //    std::cout << "worldPos.y: " << worldPos.y << std::endl;

        //    //view.setCenter(900.f, 900.f);
        //    break;
        //}
        /* Resize the window */
        case sf::Event::Resized:
        {
            /*gameView.setSize(event.size.width, event.size.height);
            guiView.setSize(event.size.width, event.size.height);
            this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), this->guiView));
            this->game->background.setScale(
                float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
                float(event.size.height) / float(this->game->background.getTexture()->getSize().y));*/
            break;
        }
        case sf::Event::MouseButtonPressed:
        {
            /* Start panning */
            if (event.mouseButton.button == sf::Mouse::Middle)
            {
                if (this->actionState != ActionState::PANNING)
                {
                    this->actionState = ActionState::PANNING;
                    this->panningAnchor = sf::Mouse::getPosition(this->game->window);
                }
            }
            break;
        }
        case sf::Event::MouseButtonReleased:
        {
            /* Stop panning */
            if (event.mouseButton.button == sf::Mouse::Middle)
            {
                this->actionState = ActionState::NONE;
            }
            break;
        }
        /* Zoom the view */
        case sf::Event::MouseWheelMoved:
        {
            if (event.mouseWheel.delta < 0)
            {
                gameView.zoom(2.0f);
                zoomLevel *= 2.0f;
            }
            else
            {
                gameView.zoom(0.5f);
                zoomLevel *= 0.5f;
            }
            break;
        }
        case sf::Event::KeyPressed:
        {
            if (event.key.code == sf::Keyboard::Escape) this->game->window.close();
            else if (event.key.code == sf::Keyboard::Space) this->loadgame();
            break;
        }
        default: break;
        }
    }

    return;
}

void GameStateEditor::loadgame()
{
    GameStateStart* gameStateEditor = new GameStateStart(this->game);
    this->game->pushState(gameStateEditor);

    return;
}



GameStateEditor::GameStateEditor(Game* game)
{
    //this->game = game;
    //sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
    //this->guiView.setSize(pos);
    //this->gameView.setSize(pos);
    //pos *= 0.5f;
    //this->guiView.setCenter(pos);
    //this->gameView.setCenter(pos);

    this->game = game;
    sf::Vector2f size = sf::Vector2f(this->game->window.getSize());
    sf::Vector2f pos = sf::Vector2f(this->game->window.getPosition());
    
    float xcoord = pos.x + size.x / 2.f;
    float ycoord = pos.y + size.y / 2.f;
    
    this->gameView.setSize(size);
    this->gameView.setCenter(sf::Vector2f(xcoord, ycoord));

    map = Map("city_map.dat", 64, 64, game->tileAtlas,game);

    this->zoomLevel = 1.0f;

    /* Centre the camera on the map */
    sf::Vector2f centre(this->map.width, this->map.height * 0.5);
    centre *= float(this->map.tileHeight);
    //gameView.setCenter(pos);

    //this->actionState = ActionState::NONE;

    sf::Vector2f viewCenter = gameView.getCenter();
    sf::Vector2f viewSize = gameView.getSize();

    //float xcoord = viewCenter.x - viewSize.x / 2.f;
    //float ycoord = viewCenter.y - viewSize.y / 2.f;

    sf::Vector2f posgame = game->background.getPosition();
    sf::Vector2f pos1 = sf::Vector2f(950.f, 0.f);
    this->game->background.setPosition(pos);
    sf::FloatRect bounds = game->background.getGlobalBounds();
    sf::FloatRect localbounds = game->background.getLocalBounds();

    this->actionState = ActionState::NONE;
}