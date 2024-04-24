#pragma once

#include "State.h"
#include "ofMain.h"
#include "Snake.h"
#include "GameState.h"

class LoseState : public State
{

public:
    LoseState();
    ~LoseState();
    void reset();
    void update();
    void draw();
    void keyPressed(int key);
};