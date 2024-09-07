#include <SFML/Graphics.hpp>

#include "game_state_start.hpp"
#include "game_state_editor.hpp"
#include "game_state.hpp"
#include <iostream>
#include "tile.hpp"

void GameStateStart::draw(const float dt)
{
    this->game->window.setView(this->view);
    //this->game->window.setView(this->guiView);

    this->game->window.clear(sf::Color::Black);

   /* sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    float xcoord = viewCenter.x - viewSize.x / 2.f;
    float ycoord = viewCenter.y - viewSize.y / 2.f;

    sf::Vector2f pos = game->background.getPosition();
    sf::Vector2f pos1 = sf::Vector2f(950.f, 0.f);
    game->background.setPosition(xcoord, ycoord);
    sf::FloatRect bounds = game->background.getGlobalBounds();
    sf::FloatRect localbounds = game->background.getLocalBounds();*/

    this->game->window.draw(this->game->background);
    this->game->window.draw(this->game->toolbar);
    //this->game->window.draw(this->brickBrushIcon);
    this->map.draw(this->game->window, dt);

    return;
}

void GameStateStart::update(const float dt)
{
}

void GameStateStart::handleInput()
{
    sf::Event event;

    while (this->game->window.pollEvent(event))
    {
        switch (event.type)
        {
            /* Close the window */
        case sf::Event::Closed:
        {
            game->window.close();
            break;
        }
        case sf::Event::MouseWheelMoved:
        {
            if (event.mouseWheel.delta < 0)
            {
                view.zoom(2.0f);
                zoomLevel *= 2.0f;
            }
            else
            {
                view.zoom(0.5f);
                zoomLevel *= 0.5f;
            }
            break;
        }
        case sf::Event::MouseMoved:
        {
            /* Pan the camera */
            if (this->actionState == RBActionState::PANNING)
            {
                sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(this->game->window) - this->panningAnchor);
                view.move(-1.0f * pos * this->zoomLevel);
                panningAnchor = sf::Mouse::getPosition(this->game->window);
            }
            else if (actionState == RBActionState::BRUSHING)
            {
                sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
                sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

                TileType tileType;
                tileType = TileType::FOREST;
                this->map.tiles.push_back(game->tileAtlas.at("forest"));
                Tile& tile = this->map.tiles.back();
                tile.sprite.setPosition(worldPos.x, worldPos.y);
            }
        }
        case sf::Event::MouseButtonPressed:
        {
            /* Start panning */
            if (event.mouseButton.button == sf::Mouse::Middle)
            {
                if (this->actionState != RBActionState::PANNING)
                {
                    this->actionState = RBActionState::PANNING;
                    this->panningAnchor = sf::Mouse::getPosition(this->game->window);
                }
            }
            
            else if (event.mouseButton.button == sf::Mouse::Left)
            {
                // get global mouse position
                if (this->actionState != RBActionState::PAINTING)
                {
                    this->actionState = RBActionState::PAINTING;
                }
                sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
                sf::Vector2i positionWindow = this->game->window.getPosition();

                // set mouse position relative to a window
                //sf::Mouse::setPosition(sf::Vector2i(100, 200), game->window);

                std::cout << "postition.x: " << position.x << std::endl;
                std::cout << "position.y: " << position.y << std::endl;

                sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

                std::cout << "worldPos.x: " << worldPos.x << std::endl;
                std::cout << "worldPos.y: " << worldPos.y << std::endl;

                TileType tileType;
                tileType = TileType::FOREST;
                this->map.tiles.push_back(game->tileAtlas.at("forest"));
                Tile& tile = this->map.tiles.back();
                tile.sprite.setPosition(worldPos.x, worldPos.y);


                //view.setCenter(900.f, 900.f);
            }
            break;
        }
        case sf::Event::MouseButtonReleased:
        {
            /* Stop panning */
            if (event.mouseButton.button == sf::Mouse::Middle)
            {
                this->actionState = RBActionState::NONE;
            }
            /* Stop selecting */
            else if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (this->actionState == RBActionState::PAINTING || this->actionState == RBActionState::BRUSHING)
                {
                    this->actionState = RBActionState::NONE;
                }
            }
            break;
        }
        /* Resize the window */
        case sf::Event::Resized:
        {
            /*this->view.setSize(event.size.width, event.size.height);
            this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
            this->game->background.setScale(
                float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
                float(event.size.height) / float(this->game->background.getTexture()->getSize().y));*/
            sf::Vector2u sizeWindow = game->window.getSize();

            std::cout << "SizeWindow.x: " << sizeWindow.x << std::endl;
            std::cout << "SizeWindow.y: " << sizeWindow.y << std::endl;

            sf::Vector2i position = sf::Mouse::getPosition();
            sf::Vector2i positionWindow = this->game->window.getPosition();

            // set mouse position relative to a window
            //sf::Mouse::setPosition(sf::Vector2i(100, 200), game->window);

            std::cout << "postition.x: " << position.x << std::endl;
            std::cout << "position.y: " << position.y << std::endl;




           /* sf::Vector2f viewCenter = view.getCenter();
            sf::Vector2f viewSize = view.getSize();

            float xcoord = viewCenter.x - viewSize.x / 2.f;
            float ycoord = viewCenter.y - viewSize.y / 2.f;

            sf::Vector2f pos = game->background.getPosition();
            sf::Vector2f pos1 = sf::Vector2f(950.f, 0.f);
            game->background.setPosition(xcoord, ycoord);
            sf::FloatRect bounds = game->background.getGlobalBounds();
            sf::FloatRect localbounds = game->background.getLocalBounds();*/

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

GameStateStart::GameStateStart(Game* game)
{
    this->game = game;
    sf::Vector2f size = sf::Vector2f(this->game->window.getSize());
    sf::Vector2f pos = sf::Vector2f(this->game->window.getPosition());

    this->brickBrushIcon.setTexture(this->game->texmgr.getRef("brickBrushIcon"));

    float xcoord = pos.x + size.x / 2.f;
    float ycoord = pos.y + size.y / 2.f;

    map = Map(this->game);
    this->zoomLevel = 1.0f;

    this->view.setSize(size);
    this->guiView.setSize(size);
    //pos *= 0.5f;
    this->view.setCenter(sf::Vector2f(xcoord, ycoord));
    this->guiView.setCenter(sf::Vector2f(xcoord, ycoord));

    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    float xcoordback = viewCenter.x - viewSize.x / 2.f;
    float ycoordback = viewCenter.y - viewSize.y / 2.f;

    sf::Vector2f posgame = game->background.getPosition();
    //sf::Vector2f pos1 = sf::Vector2f(950.f, 0.f);
    game->background.setPosition(pos);

    float toolbarOffsetPosition = RBConstants::windowHeight - RBConstants::toolbarHeight;

    game->toolbar.setPosition(pos.x, pos.y + toolbarOffsetPosition);

    this->brickBrushIcon.setPosition(pos.x + 30.f, pos.y + toolbarOffsetPosition + 30.f);

    sf::FloatRect bounds = game->background.getGlobalBounds();
    sf::FloatRect localbounds = game->background.getLocalBounds();
}

void GameStateStart::loadgame()
{
    GameStateEditor* gameStateEditor = new GameStateEditor(this->game);
    this->game->pushState(gameStateEditor);

    return;
}