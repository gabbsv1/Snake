#include "GameState.h"
int GameState::score = 0;

//--------------------------------------------------------------
GameState::GameState()
{
    cellSize = 25;
    boardSizeWidth = 64;
    boardSizeHeight = 36;

    for (int i = 0; i < boardSizeWidth; i++)
    {
        gridStatus.push_back({});
        for (int z = 0; z < boardSizeHeight; z++)
        {
            gridStatus[i].push_back({});
            gridStatus[i][z] = 0;
        }
    }
    foodSpawned = false;

    // bregando con las rocas:
    rockImage.load("rockimage.png");
    randomizeEntites();

    snake = new Snake(cellSize, boardSizeWidth, boardSizeHeight, staticEntity);

    // aqui para que el sound se mantenga escuchandose:
    sound.load("sunflower.mp3");
    sound.setLoop(true);
    sound.play();
    // aqui le doy load a la foto
    rockImage.load("rockimage.png");
}
//--------------------------------------------------------------
GameState::~GameState()
{
    delete snake;
}
//--------------------------------------------------------------
void GameState::reset()
{
    delete snake;
    randomizeEntites();
    snake = new Snake(cellSize, boardSizeWidth, boardSizeHeight, staticEntity);
    foodSpawned = false;
    sonicMode = false;
    storeCounter = 0;
    powerMode = 0;
    visitedTilesCords.clear();
    GameState::score = 0; // es para que vuelva a 0 cada vez que empieza un juego nuevo
    setFinished(false);
    setNextState("");
}
//--------------------------------------------------------------
void GameState::update()
{
    if (snake->isCrashed())
    {
        this->setNextState("LoseState");
        this->setFinished(true);
        return;
    }

    if (snake->getHead()[0] == xPos && snake->getHead()[1] == yPos)
    {
        foodSpawned = false;
        visitedTilesCords.clear();
        GameState::score += 10; // increase score
        switch (fruitType)
        {
        case NORMAL:
            snake->grow();
            break;
        case SPEED:
            sonicMode = true;
            speedCounter = 0;
            break;
        case DOUBLE:
            snake->grow();
            snake->grow();
            break;
        case GOD:
            snake->godMode = true;
            godCounter = 0;
            break;
        }
    }

    foodSpawner();

    if (ofGetFrameNum() % 1 == 0)
    {
        rotCounter += 1;
        speedCounter += 1;
        godCounter += 1;
    }

    if (sonicMode)
    {
        if (ofGetFrameNum() % 5 == 0)
            snake->update();
    }
    else
    {
        if (ofGetFrameNum() % 10 == 0)
            snake->update();
    }

    updateGrid();

    if (speedCounter % 900 == 0)
    {
        sonicMode = false;
    }

    if (godCounter % 600 == 0)
    {
        snake->godMode = false;
    }
}
//--------------------------------------------------------------
void GameState::updateGrid()
{
    for (int i = 0; i < boardSizeWidth; i++)
    {
        for (int z = 0; z < boardSizeHeight; z++)
        {
            gridStatus[i][z] = 0;
            for (unsigned int x = 1; x < snake->body.size(); x++)
            {
                if (i == snake->body[x][0] && z == snake->body[x][1])
                    gridStatus[i][z] = -1;
            }
            for (auto rock : staticEntity)
            {
                if (i == rock->getX() && z == rock->getY())
                    gridStatus[i][z] = -1;
            }
            if (xPos == i && yPos == z)
                gridStatus[i][z] = 1;
        }
    }
}
//--------------------------------------------------------------
void GameState::draw()
{
    drawBoardGrid();
    drawPath();
    snake->draw();

    if (fruitType == NORMAL && rotCounter % 60 == 0) // 1 second
    {
        red -= 4;
        green += 3;
    }

    drawFood();

    if (fruitType == NORMAL && rotCounter % 1800 == 0) // 30 seconds
    {
        foodSpawned = false;
        foodSpawner();
        findPath();
    }

    // Display the score on the screen
    ofSetColor(ofColor::white);
    string scoreStr = "Score: " + ofToString(GameState::score);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(scoreStr, 20, 30);

    string crtPow = "None";
    int crtCounter = 0;

    if (sonicMode && snake->godMode)
    {
        crtPow = "Speed and God";
        int s = (900 - speedCounter) / 60.0;
        int g = (600 - godCounter) / 60.0;
        crtCounter = (speedCounter > godCounter) ? ((900 - speedCounter) / 60.0) : (600 - godCounter) / 60.0;
    }
    else if (sonicMode)
    {
        crtPow = "Speed";
        crtCounter = (900 - speedCounter) / 60.0;
    }
    else if (snake->godMode)
    {
        crtPow = "God";
        crtCounter = (600 - godCounter) / 60.0;
    }

    ofDrawBitmapString("Current power up: " + crtPow, 20, 45);                // Current power up
    ofDrawBitmapString("Current power up: " + to_string(crtCounter), 20, 60); // Current power up counter

    string strPow = "None";
    int strCounter = 0;

    if (powerMode == SPEED)
    {
        strPow = "Speed";
        strCounter = (900 - storeCounter) / 60.0;
    }
    else if (powerMode == GOD)
    {
        strPow = "God";
        strCounter = (600 - storeCounter) / 60.0;
    }

    ofDrawBitmapString("Stored power up: " + strPow, 20, 75);                // Stored power up
    ofDrawBitmapString("Stored power up: " + to_string(strCounter), 20, 90); // Stored power up counter

    for (auto roca : staticEntity)
    {
        roca->draw();
    }
}
//--------------------------------------------------------------
void GameState::keyPressed(int key)
{
    // ofgetEla
    switch (key) // For letter keys
    {
    case 'a':
        GameState::score += 10;
        break;
    case 'b':
        powerUpStorage();
        break;
    case 'g':
        findPath();
        break;
    case 'u':
        snake->loseFat();
        break;
    case 'p':
        this->setNextState("PauseState");
        this->setFinished(true);
        return;
        break;
    }

    switch (key) // For arrow keys
    {
    case OF_KEY_LEFT:
        snake->changeDirection(LEFT);
        break;
    case OF_KEY_RIGHT:
        snake->changeDirection(RIGHT);
        break;
    case OF_KEY_UP:
        snake->changeDirection(UP);
        break;
    case OF_KEY_DOWN:
        snake->changeDirection(DOWN);
        break;
    case NONE:
        return;
        break;
    }
}
//--------------------------------------------------------------
void GameState::drawPath()
{
    ofSetColor(ofColor::ghostWhite);
    for (unsigned int i = 0; i < visitedTilesCords.size(); i++)
    {
        ofDrawRectangle(visitedTilesCords[i][0] * cellSize, visitedTilesCords[i][1] * cellSize, cellSize, cellSize);
    }
}
//--------------------------------------------------------------
void GameState::randomizeEntites()
{
    staticEntity.clear();
    for (int i = 0; i < ofRandom(15, 20); i++)
    {
        bool isInSnakeBody;
        do
        {
            isInSnakeBody = false;
            rockX = ofRandom(0, boardSizeWidth);
            rockY = ofRandom(0, boardSizeHeight);
            if ((xPos == 8 || xPos == 9 || xPos == 10 || xPos == 11) and yPos == 9)
            {
                isInSnakeBody = true;
            }
        } while (isInSnakeBody);
        staticEntity.push_back(new StaticEntity(rockX, rockY, cellSize, cellSize, rockImage));
    }
}
//--------------------------------------------------------------
void GameState::foodSpawner()
{
    if (!foodSpawned)
    {
        bool isInSnakeBody;
        do
        {
            isInSnakeBody = false;
            xPos = ofRandom(1, boardSizeWidth - 1);
            yPos = ofRandom(1, boardSizeHeight - 1);
            for (unsigned int i = 0; i < snake->getBody().size(); i++)
            {
                if (xPos == snake->getBody()[i][0] and yPos == snake->getBody()[i][1])
                {
                    isInSnakeBody = true;
                }
            }
            for (auto rock : staticEntity)
            {
                if (xPos == rock->getX() && yPos == rock->getY())
                    isInSnakeBody = true;
            }
        } while (isInSnakeBody);
        foodSpawned = true;

    if (score == 0){
    fruitType = NORMAL;
    } else if (score % 150 == 0) {
    fruitType = GOD;
    } else if (score % 100 == 0) {
        fruitType = DOUBLE;
    } else if (score % 50 == 0) {
        fruitType = SPEED;
    } else {
        fruitType = NORMAL;
}
        // fruitType = ofRandom(0, 4); // To be changed to implement activation conditions
        red = 255;
        green = 0;

        rotCounter = 0;
    }
}
//--------------------------------------------------------------
vector<int> GameState::findPath()
{
    visitedTilesCords.clear();
    return findPathHelper(snake->getHead()[0], snake->getHead()[1]);
}

vector<int> GameState::findPathHelper(int xCord, int yCord)
{
    vector<int> position = {xCord, yCord};

    if (xCord < 0 || yCord < 0 || xCord > boardSizeWidth - 1 || yCord > boardSizeHeight - 1 || gridStatus[xCord][yCord] == -1)
        return {};

    if (gridStatus[xCord][yCord] == 1)
        return position;

    vector<int> res = {};

    gridStatus[xCord][yCord] = -1;

    int yDistance = abs(yPos - yCord);
    int xDistance = abs(xPos - xCord);

    vector<string> ClosestPoint;

    if (yDistance == 0 || (xDistance != 0 && xDistance < yDistance))
    {
        if (xPos < snake->getHead()[0]) // snake on right of fruit
        {
            ClosestPoint.push_back("WEST");
            if (yPos < snake->getHead()[1]) // snake below fruit
            {
                ClosestPoint.push_back("NORTH");
                ClosestPoint.push_back("EAST");
                ClosestPoint.push_back("SOUTH");
            }
            else // snake above fruit
            {
                ClosestPoint.push_back("SOUTH");
                ClosestPoint.push_back("EAST");
                ClosestPoint.push_back("NORTH");
            }
        }
        else // snake on left of fruit
        {
            ClosestPoint.push_back("EAST");
            if (yPos < snake->getHead()[1]) // snake below fruit
            {
                ClosestPoint.push_back("NORTH");
                ClosestPoint.push_back("WEST");
                ClosestPoint.push_back("SOUTH");
            }
            else // snake above fruit
            {
                ClosestPoint.push_back("SOUTH");
                ClosestPoint.push_back("WEST");
                ClosestPoint.push_back("NORTH");
            }
        }
    }
    else if (xDistance == 0 || (yDistance != 0 && yDistance < xDistance))
    {
        if (yPos < snake->getHead()[1]) // snake below fruit
        {
            ClosestPoint.push_back("NORTH");
            if (xPos < snake->getHead()[0]) // snake to right of fruit
            {
                ClosestPoint.push_back("WEST");
                ClosestPoint.push_back("SOUTH");
                ClosestPoint.push_back("EAST");
            }
            else // snake to left of fruit
            {
                ClosestPoint.push_back("EAST");
                ClosestPoint.push_back("SOUTH");
                ClosestPoint.push_back("WEST");
            }
        }
        else // snake above fruit
        {
            ClosestPoint.push_back("SOUTH");
            if (xPos < snake->getHead()[0]) // snake to right of fruit
            {
                ClosestPoint.push_back("WEST");
                ClosestPoint.push_back("NORTH");
                ClosestPoint.push_back("EAST");
            }
            else // snake to left of fruit
            {
                ClosestPoint.push_back("EAST");
                ClosestPoint.push_back("NORTH");
                ClosestPoint.push_back("WEST");
            }
        }
    }

    for (auto codeword : ClosestPoint)
    {
        if (codeword == "EAST")
        {
            res = findPathHelper(xCord + 1, yCord); // right

            if (res.size() > 0)
            {
                visitedTilesCords.push_back(position);
                return position;
            }
        }

        if (codeword == "SOUTH")
        {
            res = findPathHelper(xCord, yCord + 1); // down

            if (res.size() > 0)
            {
                visitedTilesCords.push_back(position);
                return position;
            }
        }

        if (codeword == "WEST")
        {
            res = findPathHelper(xCord - 1, yCord); // left

            if (res.size() > 0)
            {
                visitedTilesCords.push_back(position);
                return position;
            }
        }

        if (codeword == "NORTH")
        {
            res = findPathHelper(xCord, yCord - 1); // up

            if (res.size() > 0)
            {
                visitedTilesCords.push_back(position);
                return position;
            }
        }
    }

    return {};
}
//--------------------------------------------------------------
void GameState::powerUpStorage()
{
    if (powerMode == SPEED)
    {
        sonicMode = true;
        speedCounter = storeCounter;
        powerMode = 0;
    }
    else if (powerMode == GOD)
    {
        snake->godMode = true;
        godCounter = storeCounter;
        powerMode = 0;
    }
    else
    {
        if (sonicMode)
        {
            powerMode = SPEED;
            storeCounter = speedCounter;
            sonicMode = false;
        }
        else if (snake->godMode)
        {
            powerMode = GOD;
            storeCounter = godCounter;
            snake->godMode = false;
        }
    }
}
//--------------------------------------------------------------
void GameState::drawFood()
{
    switch (fruitType) // Set color for different fruit
    {
    case NORMAL:
        ofSetColor(red, green, 0);
        break;
    case SPEED:
        ofSetColor(ofColor::cyan);
        break;
    case DOUBLE:
        ofSetColor(ofColor::plum);
        break;
    case GOD:
        ofSetColor(ofColor::goldenRod);
        break;
    }

    if (foodSpawned)
    {
        ofDrawRectangle(xPos * cellSize, yPos * cellSize, cellSize, cellSize);
    }
}
//--------------------------------------------------------------
void GameState::drawStartScreen()
{
    ofSetColor(ofColor::black);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(ofColor::white);
    string text = "Press any arrow key to start.";
    ofDrawBitmapString(text, ofGetWidth() / 2 - 8 * text.length() / 2, ofGetHeight() / 2 - 11);
    return;
}
//--------------------------------------------------------------
void GameState::drawLostScreen()
{
    reset();
    this->setNextState("LoseState");
    this->setFinished(true);
    return;
}
//--------------------------------------------------------------
void GameState::drawBoardGrid()
{

    ofDrawGrid(25, 64, false, false, false, true);

    // ofSetColor(ofColor::white);
    // for(int i = 0; i <= boardSize; i++) {
    //     ofDrawLine(i*cellSize, 0, i*cellSize, ofGetHeight());
    //     ofDrawLine(0, i*cellSize, ofGetWidth(), i*cellSize);
    // }
}
//--------------------------------------------------------------