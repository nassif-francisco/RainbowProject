#include <SFML/Graphics.hpp>

#include "game_state_start.hpp"
#include "game_state_editor.hpp"
#include "game_state.hpp"
#include <iostream>
#include "tile.hpp"
#include "hitbox.hpp"
#include <string>
#include "tinyfiledialogs.hpp"
#include "game_state_tile_splitter.hpp"

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

    //this->game->window.draw(this->game->board);

	drawBoardCollection(this->game);

    this->map.draw(this->game->window, dt, currentTileHovered, currentHitboxHovered, currentVertexHandleHovered, currentMainHandleHovered);

    this->game->window.setView(this->guiView);
    
    this->game->window.draw(this->game->toolbar);

    this->game->window.draw(this->game->mainMenu);
    
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
    
    k = 0;
    for (auto button : this->MainToolbarButtons)
    {
        if (k == 0 && currentPaintingGroundType == TileType::BACKGROUND)
        {
            button.setColor(sf::Color(255, 255, 255, 128));
        }
        else if (k == 1 && currentPaintingGroundType == TileType::FOREGROUND)
        {
            button.setColor(sf::Color(255, 255, 255, 128));
        }
        this->game->window.draw(button);
        k++;
    }

    //this->game->window.draw(this->brickBrushIcon);
    this->game->window.setView(this->view);

    return;
}

void GameStateStart::update(const float dt)
{
}

void GameStateStart::drawBoardCollection(Game* game)
{
	for (auto& board : this->game->boardCollection)
	{
		game->window.draw(board);
	}
}

void GameStateStart::executePaintingWithMaster(sf::Vector2f worldPos)
{
    if (currentBrush == nullptr)
    {
        return;
    }
    std::string currentBrushName = this->game->brushNames[*currentBrush];
    std::string tileName = currentBrushName.replace(currentBrushName.find("Button"), 6, "");


    //check if the mouse position surpasses the delegate's
    MasterPaintingDirection paintingDirection = MasterPaintingDirection::NONE;

    sf::FloatRect bounds = currentTileMasterDelegate->sprite.getGlobalBounds();
    sf::Vector2f spriteSize = bounds.getSize();

    sf::Vector2f topRightCoordinate = GetCurrentTileMasterTopRightCorner();
    sf::Vector2f bottomRightCoordinate = GetCurrentTileMasterBottomRightCorner();
    sf::Vector2f topCoordinate = GetCurrentTileMasterTopLeftCorner();
    std::string currentTileMasterID;

    if (currentTileMasterDelegate != nullptr) //currentTileMaster is dereferenced. I guess masterdelegate survives because it is assigned in each iteration.
        //remember that delegate and master were pointing to the same tile address.
    {
        currentTileMasterID = currentTileMasterDelegate->masterTileID;
    }

    if (worldPos.x > topRightCoordinate.x && worldPos.y >= topRightCoordinate.y)
    {
        paintingDirection = MasterPaintingDirection::RIGHT;
    }
    else if (worldPos.x < topCoordinate.x && worldPos.y >= topCoordinate.y)
    {
        paintingDirection = MasterPaintingDirection::LEFT;
    }
    else if (worldPos.y > bottomRightCoordinate.y && worldPos.x <= bottomRightCoordinate.x)
    {
        paintingDirection = MasterPaintingDirection::DOWN;
    }
    else if (worldPos.y < topRightCoordinate.y && worldPos.x <= topRightCoordinate.x)
    {
        paintingDirection = MasterPaintingDirection::UP;
    }




    if (paintingDirection == MasterPaintingDirection::RIGHT)
    {
        TileType tileType;
        tileType = currentPaintingGroundType;
        this->map.tiles.push_back(game->tileAtlas.at(tileName));
        Tile& tile = this->map.tiles.back();


        tile.masterTileID = currentTileMasterID;

        tile.tileMasterSlaveType = TileMasterSlaveType::SLAVE;
        currentTileMasterDelegate = &tile;

        tile.tileType = tileType;
        sf::FloatRect bounds = tile.sprite.getGlobalBounds();
        sf::Vector2f spriteSize = bounds.getSize();

        if (tile.isAnimated)
        {

            //tile.sprite.setPosition(topRightCoordinate.x - (spriteSize.x / tile.frames), topRightCoordinate.y - spriteSize.y);
        }
        else
        {
            tile.sprite.setPosition(topRightCoordinate.x, topRightCoordinate.y);
        }
    }

    if (paintingDirection == MasterPaintingDirection::LEFT)
    {
        TileType tileType;
        tileType = currentPaintingGroundType;
        this->map.tiles.push_back(game->tileAtlas.at(tileName));
        Tile& tile = this->map.tiles.back();

        tile.masterTileID = currentTileMasterID;

        tile.tileMasterSlaveType = TileMasterSlaveType::SLAVE;
        currentTileMasterDelegate = &tile;

        tile.tileType = tileType;
        sf::FloatRect bounds = tile.sprite.getGlobalBounds();
        sf::Vector2f spriteSize = bounds.getSize();

        if (tile.isAnimated)
        {

            //tile.sprite.setPosition(topRightCoordinate.x - (spriteSize.x / tile.frames) / 2, topRightCoordinate.y - spriteSize.y / 2);
        }
        else
        {
            tile.sprite.setPosition(topCoordinate.x - spriteSize.x, topCoordinate.y);
        }
    }


    if (paintingDirection == MasterPaintingDirection::DOWN)
    {
        TileType tileType;
        tileType = currentPaintingGroundType;
        this->map.tiles.push_back(game->tileAtlas.at(tileName));
        Tile& tile = this->map.tiles.back();

        tile.masterTileID = currentTileMasterID;

        tile.tileMasterSlaveType = TileMasterSlaveType::SLAVE;
        currentTileMasterDelegate = &tile;

        tile.tileType = tileType;
        sf::FloatRect bounds = tile.sprite.getGlobalBounds();
        sf::Vector2f spriteSize = bounds.getSize();

        if (tile.isAnimated)
        {

            //tile.sprite.setPosition(topRightCoordinate.x - (spriteSize.x / tile.frames) / 2, topRightCoordinate.y - spriteSize.y / 2);
        }
        else
        {
            tile.sprite.setPosition(topCoordinate.x, topCoordinate.y + spriteSize.y);
        }
    }


    if (paintingDirection == MasterPaintingDirection::UP)
    {
        TileType tileType;
        tileType = currentPaintingGroundType;
        this->map.tiles.push_back(game->tileAtlas.at(tileName));
        Tile& tile = this->map.tiles.back();

        tile.masterTileID = currentTileMasterID;

        tile.tileMasterSlaveType = TileMasterSlaveType::SLAVE;
        currentTileMasterDelegate = &tile;

        tile.tileType = tileType;
        sf::FloatRect bounds = tile.sprite.getGlobalBounds();
        sf::Vector2f spriteSize = bounds.getSize();

        if (tile.isAnimated)
        {

            //tile.sprite.setPosition(topRightCoordinate.x - (spriteSize.x / tile.frames) / 2, topRightCoordinate.y - spriteSize.y / 2);
        }
        else
        {
            tile.sprite.setPosition(topCoordinate.x, topCoordinate.y - spriteSize.y);
        }
    }


}

void GameStateStart::eraseTile()
{
    this->actionState = RBActionState::DELETING;
    this->map.tiles.erase(map.tiles.begin() + *currentTileHovered);
    currentTileHovered = nullptr;
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
                //CHECK FOR MOVING ACTION

                if (this->actionState == RBActionState::PAINTINGWITHMASTER)
                {
                    executePaintingWithMaster(worldPos);
                }
                
                if (this->actionState == RBActionState::MOVING && currentTileHovered != nullptr)
                {
                    if (DKeyPressed == true)
                    {
                        eraseTile();
                        break;

                    }             
                    
                    sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
                    sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

                    Tile& tile = this->map.tiles[*currentTileHovered];

                    sf::FloatRect bounds = tile.sprite.getGlobalBounds();
                    sf::Vector2f spriteSize = bounds.getSize();

                    float oldMasterTileXPos = bounds.getPosition().x;
                    float oldMasterTileYPos = bounds.getPosition().y;

                    tile.sprite.setPosition(worldPos.x - spriteSize.x / 2, worldPos.y - spriteSize.y / 2);

                    float newMasterTileXPos = worldPos.x - spriteSize.x / 2;
                    float newMasterTileYPos = worldPos.y - spriteSize.y / 2;

                    float deltaX = std::abs(newMasterTileXPos - oldMasterTileXPos);
                    if (newMasterTileXPos < oldMasterTileXPos)
                    {
                        deltaX *= -1;
                    }
                    float deltaY = std::abs(newMasterTileYPos - oldMasterTileYPos);
                    if (newMasterTileYPos < oldMasterTileYPos)
                    {
                        deltaY *= -1;
                    }

                    //if tile is master, move slaves
                    if (tile.tileMasterSlaveType == TileMasterSlaveType::MASTER)
                    {
                        for (auto &tiledelegate:map.tiles)
                        {
                            if (tiledelegate.tileMasterSlaveType == TileMasterSlaveType::SLAVE && tiledelegate.masterTileID == tile.masterTileID)
                            {
                                tiledelegate.sprite.setPosition(tiledelegate.sprite.getGlobalBounds().getPosition().x + deltaX, tiledelegate.sprite.getGlobalBounds().getPosition().y + deltaY);
                            }
                        }
                    }


                    break;
                }

                if (this->actionState == RBActionState::MOVING && currentMainHandleHovered != nullptr)
                {
                    sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
                    sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

                    Hitbox& hitbox = this->map.hitboxes[*currentHitboxHovered];

                    sf::Vector2f P1 = worldPos;
                    //std::vector<sf::Vector2f> hitboxPositions = {worldPos, 
                    //    sf::Vector2f(worldPos.x + 50, worldPos.y), 
                    //    sf::Vector2f(worldPos.x, worldPos.y + 50),
                    //    sf::Vector2f(worldPos.x + 50, worldPos.y + 50)};

                    //Hitbox* hitbox = new Hitbox(hitboxPositions);
                    //Hitbox currentHitbox(hitboxPositions);

                    /*this->map.hitboxes.push_back(currentHitbox);*/

                    //sf::FloatRect bounds = hitbox.Rectangle->getGlobalBounds();
                    //sf::Vector2f spriteSize = bounds.getSize();

                    float widht = std::abs(hitbox.AABB[0].x - hitbox.AABB[1].x);
                    float height = std::abs(hitbox.AABB[0].y - hitbox.AABB[3].y);

                    float rectHalfWidth = widht/2;
                    float rectHalfHeight = height/2;


                    //std::vector<sf::Vector2f> hitboxPositions = { sf::Vector2f(hitbox.AABB[0].x - deltaX, hitbox.AABB[0].x - deltaY),
                    //    sf::Vector2f(hitbox.AABB[1].x - deltaX, hitbox.AABB[1].x - deltaY),
                    //    sf::Vector2f(hitbox.AABB[2].x - deltaX, hitbox.AABB[2].x - deltaY),
                    //    sf::Vector2f(hitbox.AABB[3].x - deltaX, hitbox.AABB[3].x - deltaY) };


                    //hitbox.Rectangle->setPosition(worldPos.x - rectHalfWidth + 2.0f, worldPos.y - rectHalfHeight + 2.0f);


                   /* float handleCenterX = ((hitbox.AABB[0].x - RBConstants::VerticesSize) + (hitbox.AABB[1].x - RBConstants::VerticesSize)) / 2 - spriteSize.x/2;
                    float handleCenterY = ((hitbox.AABB[1].y - RBConstants::VerticesSize) + (hitbox.AABB[3].y - RBConstants::VerticesSize)) / 2 - spriteSize.y/2;*/

                    float handleCenterX = worldPos.x;
                    float handleCenterY = worldPos.y;
                    hitbox.MainHandle.setPosition(handleCenterX, handleCenterY);


                    std::vector<sf::Vector2f> hitboxPositions = { sf::Vector2f(handleCenterX - rectHalfWidth, handleCenterY - rectHalfHeight),
                        sf::Vector2f(handleCenterX + rectHalfWidth, handleCenterY - rectHalfHeight),
                        sf::Vector2f(handleCenterX + rectHalfWidth, handleCenterY + rectHalfHeight),
                        sf::Vector2f(handleCenterX - rectHalfWidth, handleCenterY + rectHalfHeight) };

                    hitbox.AABB = hitboxPositions;

                   /* for (int k = 0; k < hitbox.AABB.size(); k++)
                    {
                        hitbox.VertexHandles[k].setPosition({ hitbox.AABB[k].x, hitbox.AABB[k].y});
                    }*/

                    Hitbox currentHitbox(hitboxPositions, currentVertexHandleHovered);
                    //currentHitbox.Rectangle = hitbox.Rectangle;
                    hitbox = currentHitbox;
                    
                    break;
                }

                //END OF CHECK FOR MOVING

                //BEGIN CHECK FOR EDITING
                if (this->actionState == RBActionState::EDITING && currentVertexHandleHovered != nullptr)
                {
                    sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
                    sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

                    Hitbox& hitbox = this->map.hitboxes[*currentHitboxHovered];

                    AABBPoint point = static_cast<AABBPoint>(*currentVertexHandleHovered);

                    //hitbox.UpdatePoints(worldPos, point);
                    //hitbox.initializeRectangle();
                    //hitbox.updateRectangle();

                    //sf::Vector2f P1 = worldPos;
                    std::vector<sf::Vector2f> hitboxPositions;

                    if (point == AABBPoint::P1)
                    {
                        hitboxPositions = { worldPos,
                            sf::Vector2f(hitbox.AABB[1].x, worldPos.y),
                            sf::Vector2f(hitbox.AABB[2].x, hitbox.AABB[2].y),
                            sf::Vector2f(worldPos.x, hitbox.AABB[3].y) };
                    }
                    else if(point == AABBPoint::P2)
                    {
                        hitboxPositions = { sf::Vector2f(hitbox.AABB[0].x, worldPos.y),
                           worldPos,
                           sf::Vector2f(worldPos.x, hitbox.AABB[2].y),
                           sf::Vector2f(hitbox.AABB[3].x, hitbox.AABB[3].y) };
                    }
                    else if (point == AABBPoint::P3)
                    {
                        hitboxPositions = { sf::Vector2f(hitbox.AABB[0].x, hitbox.AABB[0].y),
                            sf::Vector2f(worldPos.x, hitbox.AABB[1].y),
                            worldPos,
                            sf::Vector2f(hitbox.AABB[3].x, worldPos.y) };
                    }
                    else if (point == AABBPoint::P4)
                    {
                        hitboxPositions = { sf::Vector2f(worldPos.x, hitbox.AABB[0].y),
                            sf::Vector2f(hitbox.AABB[1].x, hitbox.AABB[1].y),
                            sf::Vector2f(hitbox.AABB[2].x, worldPos.y),
                            worldPos };
                    }


                    //Hitbox* hitboxxx = new Hitbox(hitboxPositions, currentVertexHandleHovered);
                    Hitbox currentHitbox(hitboxPositions, currentVertexHandleHovered);
                    //delete hitboxxx;
                    //currentHitbox.Rectangle = hitbox.Rectangle;
                    hitbox = currentHitbox;
                   /* hitbox.AABB = hitboxPositions;
                    for (int k = 0; k < hitbox.AABB.size(); k++)
                    {
                        hitbox.VertexHandles[k].setPosition({ hitbox.AABB[k].x, hitbox.AABB[k].y });
                    }
                    hitbox.initialHoveredHandle = currentVertexHandleHovered;
                    hitbox.forceInitializeRectangle = true;*/
                    //hitbox.Rectangle = nullptr;

                    ////Hitbox* hitbox = new Hitbox(hitboxPositions);
                    ////Hitbox currentHitbox(hitboxPositions);

                    ///*this->map.hitboxes.push_back(currentHitbox);*/


                    //sf::FloatRect bounds = hitbox.Rectangle->getGlobalBounds();
                    //sf::Vector2f spriteSize = bounds.getSize();
                    //hitbox.Rectangle->setPosition(worldPos.x - spriteSize.x / 2, worldPos.y - spriteSize.y / 2);
                    //hitbox.AABB = hitboxPositions;

                    //float handleCenterX = ((hitbox.AABB[0].x - RBConstants::VerticesSize) + (hitbox.AABB[1].x - RBConstants::VerticesSize)) / 2 - spriteSize.x / 2;
                    //float handleCenterY = ((hitbox.AABB[1].y - RBConstants::VerticesSize) + (hitbox.AABB[3].y - RBConstants::VerticesSize)) / 2 - spriteSize.y / 2;

                    //hitbox.MainHandle.setPosition(handleCenterX, handleCenterY);


                    //for (int k = 0; k < hitbox.AABB.size(); k++)
                    //{
                    //    hitbox.VertexHandles[k].setPosition({ hitbox.AABB[k].x - RBConstants::VerticesSize - spriteSize.x / 2, hitbox.AABB[k].y - RBConstants::VerticesSize - spriteSize.y / 2 });
                    //}

                    break;
                }
                //END CHECK FOR EDITING
                
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
                tileType = TileType::BACKGROUND;
                this->map.tiles.push_back(game->tileAtlas.at("background"));
                Tile& tile = this->map.tiles.back();
                tile.sprite.setPosition(worldPos.x, worldPos.y);
            }
        }
        case sf::Event::MouseButtonPressed:
        {
            
            // get global mouse position
            sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
            sf::Vector2i positionWindow = this->game->window.getPosition();
            sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);        
            
            //PANNING case
            if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            {
                if (this->actionState != RBActionState::PANNING)
                {
                    this->actionState = RBActionState::PANNING;
                    this->panningAnchor = sf::Mouse::getPosition(this->game->window);
                }
                break;
            }
            
            else if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (position.y > toolbarMinY)
                {
                    //find which brush is being selected
                    setCurrentTyleID(sf::Vector2f(position.x, position.y));
                    break;
                }

                if (position.y > mainMenuMinY && position.y < mainMenuMinY + RBConstants::toolbarHeight)
                {
                    setCurrentTyleType(sf::Vector2f(position.x, position.y));
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
                if (MKeyPressed && this->actionState != RBActionState::PAINTINGWITHMASTER)
                {
                    this->actionState = RBActionState::PAINTINGWITHMASTER;
                }

                if (this->actionState != RBActionState::PAINTING && (this->actionState != RBActionState::PAINTINGWITHMASTER))
                {
                    this->actionState = RBActionState::PAINTING;
                } 
                //NOW PROCEED TO PAINT

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

                    std::vector<sf::Vector2f> hitboxPositions = { worldPos,
                        sf::Vector2f(worldPos.x + 50, worldPos.y),
                        sf::Vector2f(worldPos.x + 50, worldPos.y + 50),
                        sf::Vector2f(worldPos.x, worldPos.y + 50) };

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
                tileType = currentPaintingGroundType;
                this->map.tiles.push_back(game->tileAtlas.at(tileName));
                Tile& tile = this->map.tiles.back();

                currentTileMaster = &tile;
                currentTileMasterDelegate = &tile;

                tile.tileMasterSlaveType = TileMasterSlaveType::MASTER;
                tile.masterTileID = generateRandomString();

                tile.tileType = tileType;
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
            else if (event.mouseButton.button == sf::Mouse::Right)
            {
                if (checkIfMouseClickIsOnMainHandle(worldPos))
                {
                    this->actionState = RBActionState::TAGGING;
                    char* newTag;
                    newTag = tinyfd_inputBox(
                        "Give your hitbox a name", NULL, NULL);

                    if (newTag == NULL)
                    {
                        break;
                    }

                    Hitbox& hitbox = this->map.hitboxes[*currentHitboxHovered];
                    hitbox.Tag = newTag;
                    break;
                }
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
                    || this->actionState == RBActionState::MOVING
                    || this->actionState == RBActionState::EDITING
                    || this->actionState == RBActionState::PAINTINGWITHMASTER)
                {
                    this->actionState = RBActionState::NONE;
                    currentTileMaster = nullptr;
                    currentTileMasterDelegate = nullptr;
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

			float currentToolbarHeightFactor = GetCurrentToolbarHeightFactor();

            float toolbarOffsetPosition = (height - RBConstants::toolbarHeight) - (height - RBConstants::toolbarHeight)/ currentToolbarHeightFactor;
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
            else if (event.key.code == sf::Keyboard::LControl) this->LControlKeyPressed = true;
            else if (event.key.code == sf::Keyboard::S) this->SKeyPressed = true;
            else if (event.key.code == sf::Keyboard::M) this->MKeyPressed = true;
            else if (event.key.code == sf::Keyboard::D) this->DKeyPressed = true;
            else if (event.key.code == sf::Keyboard::L) this->LKeyPressed = true;
            else if (event.key.code == sf::Keyboard::T) this->TKeyPressed = true;
            else if (event.key.code == sf::Keyboard::H)
            {
                currentTileHovered = nullptr;
            }
            

            if (LControlKeyPressed && SKeyPressed)
            {
				std::string filename = RBConstants::CommonMediaMapsPath + "test";
                map.save(filename);
                SKeyPressed = false;
                LControlKeyPressed = false;
            }

            if (LControlKeyPressed && LKeyPressed)
            {
                std::string filename = RBConstants::CommonMediaMapsPath + "test";
                map.load(filename);
                LKeyPressed = false;
                LControlKeyPressed = false;
            }

            if (LControlKeyPressed && TKeyPressed)
            {
                TKeyPressed = false;
                LControlKeyPressed = false;

                loadTileSplitter();
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
            else if (event.key.code == sf::Keyboard::S)
            {
                SKeyPressed = false;
            }
            else if (event.key.code == sf::Keyboard::L)
            {
                LKeyPressed = false;
            }
            else if (event.key.code == sf::Keyboard::M)
            {
                MKeyPressed = false;
            }
            else if (event.key.code == sf::Keyboard::D)
            {
                DKeyPressed = false;
            }
            else if (event.key.code == sf::Keyboard::LControl)
            {
                LControlKeyPressed = false;
            }
            break;
        }
        default: break;
        }
    }

    return;
}

void GameStateStart::assembleBoardCollection(Game* game, MapGrid& mapGrid)
{
    sf::Sprite boardSprite = game->board;
    
    for (int k = 0; k < mapGrid.RepetitionFactor; k++) //repetition along y axis
    {
        for (int m = 0; m < mapGrid.RepetitionFactor; m++) // rep along x axis
        {
			double xPosition = mapGrid.UpperLeftCornerPosition.x + (m * mapGrid.TileSizeX);
			double yPosition = mapGrid.UpperLeftCornerPosition.y + (k * mapGrid.TileSizeY);
            
            sf::Sprite newBoardSprite = boardSprite;
			newBoardSprite.setPosition(xPosition, yPosition);
			game->boardCollection.push_back(newBoardSprite);
        }
    }
}

void GameStateStart::assembleToolbar(Game* game, sf::Vector2f pos, sf::Vector2f size)
{
    float xcoord = pos.x + size.x / 2.f;
    float ycoord = pos.y + size.y / 2.f;
    float currentToolbarHeightFactor = GetCurrentToolbarHeightFactor();

    float toolbarOffsetPosition = size.y - size.y / currentToolbarHeightFactor;
    
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

    size_t totalTiles = game->tileNames.size();

    for (int i =0; i < totalTiles; i++)
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

void GameStateStart::assembleMainMenu(Game* game, sf::Vector2f pos, sf::Vector2f size)
{
    float xcoord = pos.x + size.x / 2.f;
    float ycoord = pos.y + size.y / 2.f;


    float toolbarOffsetPosition = 0;

    //float toolbarOffsetPosition = RBConstants::windowHeight - RBConstants::toolbarHeight;
    mainMenuMinY = toolbarOffsetPosition;

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
    float brushWidth = 60.f;
    float brushPositionY = pos.y + brushDistance + toolbarOffsetPosition;

    for (int i = 0; i <= 1; i++)
    {
        brushPositionX += brushDistance + brushWidth;
        sf::Sprite* newSprite = new sf::Sprite();
        MainToolbarButtons.push_back(*newSprite);

        this->MainToolbarButtons[i].setTexture(this->game->texmgr.getRef(this->game->buttonNames[i]));

        this->MainToolbarButtons[i].setPosition(brushPositionX, pos.y + brushDistance + toolbarOffsetPosition);
    }

    //for (size_t i = 21; i <= this->game->brushNames.size(); i++)
    //{


    //}


    //this->brickBrushIcon.setTexture(this->game->texmgr.getBrushRef("FlowerPot4YELLOWButton"));

    //sf::FloatRect floatrect = this->brickBrushIcon.getLocalBounds();

    game->mainMenu.setPosition(pos.x, pos.y);

    //this->guiView.setSize(size);
    //this->guiView.setCenter(sf::Vector2f(xcoord, ycoord));

    ////this->brickBrushIcon.setPosition(pos.x + 10.f, pos.y + toolbarOffsetPosition + 10.f);

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

void GameStateStart::setCurrentTyleType(sf::Vector2f position)
{
    int b = 0;
    for (auto button : this->MainToolbarButtons)
    {
        sf::FloatRect boundingBox = button.getGlobalBounds();

        if (boundingBox.contains((sf::Vector2f)position))
        {
            if (b == 0)
            {
                currentPaintingGroundType = TileType::BACKGROUND;
            }
            else
            {
                currentPaintingGroundType = TileType::FOREGROUND;
            }
            button.setColor(sf::Color(255, 255, 255, 128));
        }
        b++;
    }
}

void GameStateStart::updateObjects()
{
    game->loadTextures();
    game->loadTiles();

    sf::Vector2f size = sf::Vector2f(this->game->window.getSize());
    sf::Vector2f pos = sf::Vector2f(this->game->window.getPosition());

    assembleToolbar(game, pos, size);
}

GameStateStart::GameStateStart(Game* game)
{
    this->game = game;
    sf::Vector2f size = sf::Vector2f(this->game->window.getSize());
    sf::Vector2f pos = sf::Vector2f(this->game->window.getPosition());

    float xcoord = pos.x + size.x / 2.f;
    float ycoord = pos.y + size.y / 2.f;

    map = Map(this->game);
    this->zoomLevel = 1.201f;

    this->view.setSize(size);
    //view.zoom(0.3);

    //pos *= 0.5f;
    this->view.setCenter(sf::Vector2f(xcoord, ycoord));

    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    float xcoordback = viewCenter.x - viewSize.x / 2.f;
    float ycoordback = viewCenter.y - viewSize.y / 2.f;

    sf::Vector2f posgame = game->background.getPosition();
	sf::Vector2i mapUpperLeftPosition = MapGrid::GetMapUpperLeftCornerPosition(this->game->window);
    
    //assemble board pattern
    sf::FloatRect boundsSprite = game->board.getGlobalBounds();
    sf::Vector2f spriteSize = boundsSprite.getSize();
	MapGrid mapGrid = MapGrid(mapUpperLeftPosition, spriteSize.x, spriteSize.y, 10);
    //board pattern


    //sf::Vector2f pos1 = sf::Vector2f(950.f, 0.f);
    game->background.setPosition(pos);
    game->board.setPosition(viewCenter.x - 378.5, viewCenter.y - 378.5);

	assembleBoardCollection(game, mapGrid);
    assembleToolbar(game, pos, size);
    assembleMainMenu(game, pos, size);

    sf::FloatRect bounds = game->background.getGlobalBounds();
    sf::FloatRect localbounds = game->background.getLocalBounds();
}

void GameStateStart::loadTileSplitter()
{
    GameStateTileSplitter* gameStateTileSplitter = new GameStateTileSplitter(this->game);
    this->game->pushState(gameStateTileSplitter);

    return;
}