#include <SFML/Graphics.hpp>

#include "game_state_tile_splitter.hpp"
#include "game_state_editor.hpp"
#include "game_state.hpp"
#include <iostream>
#include "tile.hpp"
#include "hitbox.hpp"
#include <string>
#include "tinyfiledialogs.hpp"


void GameStateTileSplitter::draw(const float dt)
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

    //this->game->window.draw(this->brickBrushIcon);
    this->game->window.setView(this->view);

    return;
}

void GameStateTileSplitter::update(const float dt)
{
}

void GameStateTileSplitter::drawBoardCollection(Game* game)
{
    for (auto& board : this->game->boardCollection)
    {
        game->window.draw(board);
    }
}

void GameStateTileSplitter::handleInput()
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

                if (this->actionState == RBActionState::MOVING && currentTileHovered != nullptr)
                {
                    sf::Vector2i position = sf::Mouse::getPosition(this->game->window);
                    sf::Vector2f worldPos = this->game->window.mapPixelToCoords(position);

                    Tile& tile = this->map.tiles[*currentTileHovered];

                    sf::FloatRect bounds = tile.sprite.getGlobalBounds();
                    sf::Vector2f spriteSize = bounds.getSize();
                    //tile.sprite.setPosition(worldPos.x - spriteSize.x / 2, worldPos.y - spriteSize.y / 2);
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

                    float rectHalfWidth = widht / 2;
                    float rectHalfHeight = height / 2;


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
                    else if (point == AABBPoint::P2)
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
                    break;
                }

                if (position.y > mainMenuMinY && position.y < mainMenuMinY + RBConstants::toolbarHeight)
                {

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

                std::string currentBrushName = "AABBButton";
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
                    || this->actionState == RBActionState::EDITING)
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

            float currentToolbarHeightFactor = GetCurrentToolbarHeightFactor();

            float toolbarOffsetPosition = (height - RBConstants::toolbarHeight) - (height - RBConstants::toolbarHeight) / currentToolbarHeightFactor;
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
            else if (event.key.code == sf::Keyboard::O) this->OKeyPressed = true;
            else if (event.key.code == sf::Keyboard::H)
            {
                currentTileHovered = nullptr;
            }


            if (LControlKeyPressed && SKeyPressed)
            {
                createTilesUsingHitboxes(tileSetFileName);
                loadgame();
            }

            if (LControlKeyPressed && OKeyPressed)
            {
                char const* lFilterPatterns[2] = { "*.png", "*.jpg" };
                char* tilesetFileName = tinyfd_openFileDialog(
                    "select your tileset",
                    "../",
                    2,
                    lFilterPatterns,
                    "image files",
                    1);

                if (tilesetFileName == NULL)
                {

                    OKeyPressed = false;
                    LControlKeyPressed = false;
                    
                    return;
                }

                std::string fileName(tilesetFileName);
                tileSetFileName = fileName;

                insertTileSetInScreen(fileName);

                //createTilesUsingHitboxes(fileName);

                OKeyPressed = false;
                LControlKeyPressed = false;

                return;

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
            else if (event.key.code == sf::Keyboard::O)
            {
                OKeyPressed = false;
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

void GameStateTileSplitter::assembleBoardCollection(Game* game, MapGrid& mapGrid)
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

void GameStateTileSplitter::assembleToolbar(Game* game, sf::Vector2f pos, sf::Vector2f size)
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

void GameStateTileSplitter::assembleMainMenu(Game* game, sf::Vector2f pos, sf::Vector2f size)
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

bool GameStateTileSplitter::checkIfMousePositionIsOnTile(sf::Vector2f position)
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

bool GameStateTileSplitter::checkIfMouseClickIsOnTile(sf::Vector2f position)
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

bool GameStateTileSplitter::checkIfMouseClickIsOnVertexHandle(sf::Vector2f position)
{
    int b = 0;
    for (auto hitbox : this->map.hitboxes)
    {
        int h = 0;
        for (auto handle : hitbox.VertexHandles)
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

bool GameStateTileSplitter::checkIfMouseClickIsOnMainHandle(sf::Vector2f position)
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




void GameStateTileSplitter::setCurrentTyleID(sf::Vector2f position)
{
    int b = 0;
}

void GameStateTileSplitter::setCurrentTyleType(sf::Vector2f position)
{
    int b = 0;
   
}

void GameStateTileSplitter::updateObjects()
{

}

GameStateTileSplitter::GameStateTileSplitter(Game* game)
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

void GameStateTileSplitter::loadgame()
{
    this->game->popState();

    return;
}

void GameStateTileSplitter::insertDefaultHitboxesForSprites(string filename, float spritewidth, float spriteheight)
{
   
    std::vector<int> numbers;

    for (size_t pos = 0; (pos = filename.find('_', pos)) != std::string::npos; ++pos) {
        if (pos + 1 < filename.size() && std::isdigit(filename[pos + 1])) {
            numbers.push_back(filename[pos + 1] - '0');
        }
    }

    if (numbers.size() == 0)
    {
        return;
    }

    if (numbers.size() >= 1) {
        verticalFrames = numbers[0];
    }
    if (numbers.size() >= 2) {
        horizontalFrames = numbers[1];
    }

    float currentSpriteWidht = spritewidth / 1;
    float currentSpriteHeight = spriteheight / verticalFrames;

    sf::Vector2f worldPos = sf::Vector2f(XCornerTileSet, YCornerTileSet);

    for (int k= 0; k < verticalFrames; k++)
    { 
        std::vector<sf::Vector2f> hitboxPositions = { worldPos,
            sf::Vector2f(worldPos.x + currentSpriteWidht, worldPos.y),
            sf::Vector2f(worldPos.x + currentSpriteWidht, worldPos.y + currentSpriteHeight),
            sf::Vector2f(worldPos.x, worldPos.y + currentSpriteHeight) };

        //Hitbox* hitbox = new Hitbox(hitboxPositions);
        Hitbox currentHitbox(hitboxPositions);

        this->map.hitboxes.push_back(currentHitbox);

        worldPos.y += currentSpriteHeight;

    }

}

void GameStateTileSplitter::insertTileSetInScreen(string tilesetFileName)
{
    std::string fileName(tilesetFileName);
    //load the texture
    game->texmgr.loadTexture(fileName, fileName);

    //std::string tileName = fileName.replace(fileName.find(".png"), 4, "");
    game->tileNames.push_back(fileName);
    game->loadTiles();

    this->map.tiles.push_back(game->tileAtlas.at(fileName));
    Tile& tile = this->map.tiles.back();
    tile.tileType = TileType::FOREGROUND;
    sf::FloatRect bounds = tile.sprite.getGlobalBounds();
    sf::Vector2f spriteSize = bounds.getSize();

    float spritewidth = tile.sprite.getGlobalBounds().getSize().x / 2;
    float spriteheight = tile.sprite.getGlobalBounds().getSize().y / 2;

    sf::Vector2f size = sf::Vector2f(this->game->window.getSize());
    sf::Vector2f pos = sf::Vector2f(this->game->window.getPosition());

    float xcoord = pos.x + size.x / 2.f - spritewidth;
    float ycoord = pos.y + size.y / 2.f - spriteheight;
    XCornerTileSet = xcoord;
    YCornerTileSet = ycoord;

    tile.sprite.setPosition(xcoord, ycoord);

    insertDefaultHitboxesForSprites(fileName, spritewidth*2, spriteheight*2);

    //check if tilename contains "_" character

    
}

void GameStateTileSplitter::createTilesUsingHitboxes(string fileName)
{
    for (auto& hitbox : map.hitboxes)
    {
        sf::Image sourceImage;
        if (!sourceImage.loadFromFile(fileName))
        {
            std::cerr << "Failed to load image\n";

            OKeyPressed = false;
            LControlKeyPressed = false;

            return;
        }

        float width = std::abs(hitbox.AABB[0].x - hitbox.AABB[1].x);
        float height = std::abs(hitbox.AABB[0].y - hitbox.AABB[3].y);


        // Define the portion you want to extract
        sf::IntRect rect(hitbox.AABB[0].x - XCornerTileSet, hitbox.AABB[0].y - YCornerTileSet, width, height); // x, y, width, height

        // Create a new image with the same size as the portion
        sf::Image subImage;
        subImage.create(rect.width, rect.height, sf::Color::Transparent);

        // Copy pixels from the source image into the new one
        subImage.copy(sourceImage, 0, 0, rect, true);

        // Save the extracted part as a new PNG
        string finalPathNewTileBase = RBConstants::CommonMediaEnvironmentPacksPath + "Toolbar/";

        ////////////////////////////////////
        string guidStr = generateRandomString();
        ///////////////////////////////
        string finalPathNewTile;

        if (verticalFrames > 0) //then it is a sprite
        {
            finalPathNewTile = finalPathNewTileBase + guidStr + "Sprite" + "_" + std::to_string(horizontalFrames) +".png";
        }
        else
        {
            finalPathNewTile = finalPathNewTileBase + guidStr + ".png";
        }

        if (!subImage.saveToFile(finalPathNewTile))
        {
            std::cerr << "Failed to save image\n";
            OKeyPressed = false;
            LControlKeyPressed = false;
            return;
        }


        /////////////////////////////
        // Convert the sub-image into a texture
        sf::Texture texture;
        texture.loadFromImage(subImage);

        // Create a sprite for resizing
        sf::Sprite sprite(texture);

        // Desired new size
        unsigned newWidth = 32;
        unsigned newHeight = 32;

        // Calculate scale factors
        sprite.setScale(
            static_cast<float>(newWidth) / rect.width,
            static_cast<float>(newHeight) / rect.height
        );

        // Render the scaled sprite into a new texture
        sf::RenderTexture renderTexture;
        renderTexture.create(newWidth, newHeight);
        renderTexture.clear(sf::Color::Transparent);
        renderTexture.draw(sprite);
        renderTexture.display();

        // Get the resized image
        sf::Image resizedImage = renderTexture.getTexture().copyToImage();

        string finalPathNewTileIcon;

        if (verticalFrames > 0) //then it is a sprite
        {
            finalPathNewTileIcon = finalPathNewTileBase + guidStr + "Sprite" + "_" + std::to_string(horizontalFrames) + "Button.png";
        }
        else
        {
            finalPathNewTileIcon = finalPathNewTileBase + guidStr + "Button.png";
        }

        // Save to file
        if (!resizedImage.saveToFile(finalPathNewTileIcon))
        {
            std::cerr << "Failed to save resized image\n";
            OKeyPressed = false;
            LControlKeyPressed = false;
            return;
        }
    }

    game->tileNames.pop_back();
}
