#include <SFML/Graphics.hpp>

#include "game_state_start.hpp"
#include "game_state_editor.hpp"
#include "game_state.hpp"
#include <iostream>
#include "tile.hpp"
#include "hitbox.hpp"
#include <string>

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

    this->game->window.draw(this->game->board);

    this->map.draw(this->game->window, dt, currentTileHovered, currentHitboxHovered, currentVertexHandleHovered, currentMainHandleHovered);

    this->game->window.draw(this->game->toolbar);
    
    int k = 0;
    for (auto brush : this->Row1Brushes)
    {
        if (currentBrush != nullptr && *currentBrush == k)
        {
            brush.setColor(sf::Color(255, 255, 255, 128));
        }
        this->game->window.draw(brush);
        k++;
    }

    for (auto brush : this->Row2Brushes)
    {
        this->game->window.draw(brush);
    }

    //this->game->window.draw(this->brickBrushIcon);
    

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
            sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
            sf::Vector2i positionWindow = this->game->window.getPosition();
            sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

            if (position.y < toolbarMinY)
            {
                if (this->actionState == RBActionState::MOVING && currentTileHovered != nullptr)
                {
                    sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
                    sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

                    Tile& tile = this->map.tiles[*currentTileHovered];

                    sf::FloatRect bounds = tile.sprite.getGlobalBounds();
                    sf::Vector2f spriteSize = bounds.getSize();
                    tile.sprite.setPosition(worldPos.x - spriteSize.x / 2, worldPos.y - spriteSize.y / 2);
                    break;
                }
                
                //find which brush is being selected
                //checkIfMousePositionIsOnTile(worldPos);
                //break;

                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::H))
                {
                    if (checkIfMouseClickIsOnTile(worldPos))
                    {
                        break;
                    }
                }
                else
                {
                    if (checkIfMouseClickIsOnVertexHandle(worldPos))
                    {
                        break;
                    }

                    if (checkIfMouseClickIsOnMainHandle(worldPos))
                    {
                        break;
                    }
                }
            }

            //if (this->actionState != RBActionState::MOVING)
            //{
            //    this->actionState = RBActionState::MOVING;
            //}



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
                sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
                sf::Vector2i positionWindow = this->game->window.getPosition();
                sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

                if (worldPos.y > toolbarMinY)
                {
                    //find which brush is being selected
                    setCurrentTyleID(worldPos);
                    break;
                }

                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::H))
                {
                    if (checkIfMouseClickIsOnTile(worldPos))
                    {
                        this->actionState = RBActionState::MOVING;
                        break;
                    }
                }
                else
                {
                    if (checkIfMouseClickIsOnVertexHandle(worldPos))
                    {
                        this->actionState = RBActionState::EDITING;
                        break;
                    }

                    if (checkIfMouseClickIsOnMainHandle(worldPos))
                    {
                        this->actionState = RBActionState::MOVING;
                        break;
                    }
                }

                //check if current position is over any existing tile
                /*if (checkIfMouseClickIsOnTile(worldPos))
                {
                    this->actionState = RBActionState::MOVING;
                    break;
                }*/

                if (this->actionState != RBActionState::PAINTING)
                {
                    this->actionState = RBActionState::PAINTING;
                } 

                // set mouse position relative to a window
                //sf::Mouse::setPosition(sf::Vector2i(100, 200), game->window);

                std::cout << "postition.x: " << position.x << std::endl;
                std::cout << "position.y: " << position.y << std::endl;

                

                std::cout << "worldPos.x: " << worldPos.x << std::endl;
                std::cout << "worldPos.y: " << worldPos.y << std::endl;

                if (currentBrush == nullptr)
                {
                    return;
                }
                std::string currentBrushName = this->game->brushNames[*currentBrush];
                std::string tileName = currentBrushName.replace(currentBrushName.find("Button"), 6, "");

                if (tileName == "AABB")
                {
                    sf::Vector2f P1 = worldPos;
                    std::vector<sf::Vector2f> hitboxPositions = {worldPos, 
                        sf::Vector2f(worldPos.x + 50, worldPos.y), 
                        sf::Vector2f(worldPos.x, worldPos.y + 50),
                        sf::Vector2f(worldPos.x + 50, worldPos.y + 50)};

                    //Hitbox* hitbox = new Hitbox(hitboxPositions);
                    Hitbox currentHitbox(hitboxPositions);

                    this->map.hitboxes.push_back(currentHitbox);
                    break;
                    //this->map.hitboxes.push_back(*hitbox);
                    //sf::RectangleShape rectangle({ 120.f, 50.f });

                    //// change the size to 100x100
                    //rectangle.setSize({ 100.f, 100.f });
                }
                
                TileType tileType;
                tileType = TileType::FOREST;
                this->map.tiles.push_back(game->tileAtlas.at(tileName));
                Tile& tile = this->map.tiles.back();
                sf::FloatRect bounds = tile.sprite.getGlobalBounds();
                sf::Vector2f spriteSize = bounds.getSize();

                if (tile.isAnimated)
                {
                   
                    tile.sprite.setPosition(worldPos.x - (spriteSize.x /tile.frames)/2, worldPos.y - spriteSize.y / 2);
                }
                else
                {
                    tile.sprite.setPosition(worldPos.x - spriteSize.x / 2, worldPos.y - spriteSize.y / 2); 
                }

               

                currentTileHovered = new int(map.tiles.size() - 1);
                

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
                if (this->actionState == RBActionState::PAINTING 
                    || this->actionState == RBActionState::BRUSHING
                    || this->actionState == RBActionState::MOVING)
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
            sf::Vector2i positionWindow = this->game->window.getPosition();
            sf::Vector2u sizeWindow = game->window.getSize();

            float height = sf::VideoMode::getDesktopMode().height;

            float toolbarOffsetPosition = (height - RBConstants::toolbarHeight) - (height - RBConstants::toolbarHeight)/RBConstants::toolbarHeightFactor;
            toolbarMinY = toolbarOffsetPosition;

            std::cout << "SizeWindow.x: " << sizeWindow.x << std::endl;
            std::cout << "SizeWindow.y: " << sizeWindow.y << std::endl;

            sf::Vector2i position = sf::Mouse::getPosition();
            

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
            else if (event.key.code == sf::Keyboard::H)
            {
                currentTileHovered = nullptr;
            }
            break;
        }
        case sf::Event::KeyReleased:
        {
            
            if (event.key.code == sf::Keyboard::H)
            {
                currentTileHovered = nullptr;
                currentHitboxHovered = nullptr;
                currentVertexHandleHovered = nullptr;
                currentMainHandleHovered = nullptr;
            }
            break;
        }
        default: break;
        }
    }

    return;
}

void GameStateStart::assembleToolbar(Game* game, sf::Vector2f pos, sf::Vector2f size)
{
    float xcoord = pos.x + size.x / 2.f;
    float ycoord = pos.y + size.y / 2.f;


    float toolbarOffsetPosition = size.y - size.y / RBConstants::toolbarHeightFactor;
    
    //float toolbarOffsetPosition = RBConstants::windowHeight - RBConstants::toolbarHeight;
    toolbarMinY = toolbarOffsetPosition;

    //load pack
    //check everything in Packs/Forest folder
    //load each tuple in a vector, pack icons
    //set pack region--> extract first 42 elements from pack
    //draw elements

   /* for (const std::string& brushIcon : game->brushNames) {
        this->brickBrushIcon.setTexture(this->game->texmgr.getRef(brushIcon));
    }*/
    float brushPositionX = pos.x;
    float brushDistance = 10.f;
    float brushWidth = 32.f;
    float brushPositionY = pos.y + brushDistance + toolbarOffsetPosition;

    for (int i =0; i<=8; i++)
    {
        brushPositionX += brushDistance + brushWidth;
        sf::Sprite* newSprite =  new sf::Sprite();
        Row1Brushes.push_back(*newSprite);

        this->Row1Brushes[i].setTexture(this->game->texmgr.getBrushRef(this->game->brushNames[i]));

        this->Row1Brushes[i].setPosition(brushPositionX, pos.y + brushDistance + toolbarOffsetPosition);
    }

    //for (size_t i = 21; i <= this->game->brushNames.size(); i++)
    //{


    //}


    //this->brickBrushIcon.setTexture(this->game->texmgr.getBrushRef("FlowerPot4YELLOWButton"));

    //sf::FloatRect floatrect = this->brickBrushIcon.getLocalBounds();

    game->toolbar.setPosition(pos.x, pos.y + toolbarOffsetPosition);

    this->guiView.setSize(size);
    this->guiView.setCenter(sf::Vector2f(xcoord, ycoord));

    //this->brickBrushIcon.setPosition(pos.x + 10.f, pos.y + toolbarOffsetPosition + 10.f);

}

bool GameStateStart::checkIfMousePositionIsOnTile(sf::Vector2f position)
{
    int b = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
    {
        return false;
    }

    for (auto tile : this->map.tiles)
    {
        sf::FloatRect boundingBox = tile.sprite.getGlobalBounds();

        if (boundingBox.contains((sf::Vector2f)position))
        {
            currentTileHovered = new int(b);
            return true;
        }
        b++;
    }
    currentTileHovered = nullptr;
    return false;
}

bool GameStateStart::checkIfMouseClickIsOnTile(sf::Vector2f position)
{
    int b = 0;
    for (auto tile : this->map.tiles)
    {
        sf::FloatRect boundingBox = tile.sprite.getGlobalBounds();

        if (boundingBox.contains((sf::Vector2f)position))
        {
            currentTileHovered = new int(b);
            return true;
        }
        b++;
    }
    currentTileHovered = nullptr;
    return false;
}

bool GameStateStart::checkIfMouseClickIsOnVertexHandle(sf::Vector2f position)
{
    int b = 0;
    for (auto hitbox : this->map.hitboxes)
    {     
        int h = 0;
        for (auto handle: hitbox.VertexHandles)
        {
            sf::FloatRect boundingBox = handle.getGlobalBounds();

            if (boundingBox.contains((sf::Vector2f)position))
            {
                currentHitboxHovered = new int(b);
                currentVertexHandleHovered = new int(h);
                return true;
            }
            h++;        
        }
        b++;
    }
    currentHitboxHovered = nullptr;
    currentVertexHandleHovered = nullptr;

    return false;
}

bool GameStateStart::checkIfMouseClickIsOnMainHandle(sf::Vector2f position)
{
    int b = 0;
    for (auto hitbox : this->map.hitboxes)
    {

        sf::FloatRect boundingBox = hitbox.MainHandle.getGlobalBounds();

        if (boundingBox.contains((sf::Vector2f)position))
        {
            currentHitboxHovered = new int(b);
            currentMainHandleHovered = new int(0);
            return true;
        }

        b++;
    }
    currentHitboxHovered = nullptr;
    currentMainHandleHovered = nullptr;

    return false;
}




void GameStateStart::setCurrentTyleID(sf::Vector2f position)
{
    int b = 0;
    for (auto brush : this->Row1Brushes)
    {
        sf::FloatRect boundingBox = brush.getGlobalBounds();

        if (boundingBox.contains((sf::Vector2f)position))
        {
            currentBrush = new int(b);
            brush.setColor(sf::Color(255, 255, 255, 128));
        }
        b++;
    }
}

GameStateStart::GameStateStart(Game* game)
{
    this->game = game;
    sf::Vector2f size = sf::Vector2f(this->game->window.getSize());
    sf::Vector2f pos = sf::Vector2f(this->game->window.getPosition());

    float xcoord = pos.x + size.x / 2.f;
    float ycoord = pos.y + size.y / 2.f;

    map = Map(this->game);
    this->zoomLevel = 1.0f;

    this->view.setSize(size);

    //pos *= 0.5f;
    this->view.setCenter(sf::Vector2f(xcoord, ycoord));

    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    float xcoordback = viewCenter.x - viewSize.x / 2.f;
    float ycoordback = viewCenter.y - viewSize.y / 2.f;

    sf::Vector2f posgame = game->background.getPosition();
    //sf::Vector2f pos1 = sf::Vector2f(950.f, 0.f);
    game->background.setPosition(pos);
    game->board.setPosition(viewCenter.x - 378.5, viewCenter.y - 378.5);

    assembleToolbar(game, pos, size);

    sf::FloatRect bounds = game->background.getGlobalBounds();
    sf::FloatRect localbounds = game->background.getLocalBounds();
}

void GameStateStart::loadgame()
{
    GameStateEditor* gameStateEditor = new GameStateEditor(this->game);
    this->game->pushState(gameStateEditor);

    return;
}