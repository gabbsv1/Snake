#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowTitle("Snake");

    gameState = new GameState();
    menuState = new MenuState();
    pauseState = new PauseState();
    loseState = new LoseState();
    currentState = menuState;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (currentState->hasFinished())
    {
        if (currentState->getNextState() == "GameState")
        {
            if (currentState == pauseState)
            {
                gameState->setFinished(false);
                gameState->setNextState("");
            }
            else
                gameState->reset();
            currentState = gameState;
        }
        else if (currentState->getNextState() == "MenuState")
        {
            menuState->reset();
            currentState = menuState;
        }
        else if (currentState->getNextState() == "LoseState")
        {
            loseState->reset();
            currentState = loseState;
        }
        else if (currentState->getNextState() == "PauseState")
        {
            pauseState->reset();
            currentState = pauseState;
        }
    }
    currentState->update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    currentState->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    currentState->keyPressed(key);
}
//--------------------------------------------------------------