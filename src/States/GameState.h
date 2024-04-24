#pragma once

#include "State.h"
#include "Snake.h"
#include "ofMain.h"
#include "PauseState.h"
#include "ofSoundPlayer.h"
#include "StaticEntity.h"

enum FruitType
{
    NORMAL,
    SPEED,
    DOUBLE,
    GOD
};

class GameState : public State
{
public:
    GameState();
    ~GameState();
    void reset();
    void update();
    void draw();
    void keyPressed(int key);
    void foodSpawner();
    void powerUpStorage();
    void drawFood();
    void drawPath();
    void updateGrid();
    vector<int> findPath();
    vector<int> findPathHelper(int xCord, int yCord);
    void drawStartScreen();
    void drawLostScreen();
    void drawBoardGrid();
    void drawRocks();

    Snake *snake;
    
    vector<vector<int>> gridStatus;
    vector<vector<int>> visitedTilesCords;

    bool foodSpawned = false;
    int fruitType;

    int rotCounter = 0;
    bool sonicMode = false;
    int speedCounter = 0;
    int godCounter = 0;

    int storeCounter;
    int powerMode;

    int red = 255;
    int green = 0;

    int xPos;
    int yPos;
    int rockX;
    int rockY;

    int boardSizeWidth, boardSizeHeight;
    int cellSize; // Pixels
    static int score;

    ofImage rockImage;

    //vector para las rocas:
    vector<StaticEntity*> staticEntity;
    void randomizeEntites();

    ofSoundPlayer sound;
    string currentSong;
};