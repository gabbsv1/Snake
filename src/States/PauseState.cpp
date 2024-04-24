#include "PauseState.h"

//--------------------------------------------------------------
PauseState::PauseState()
{
}
//--------------------------------------------------------------
PauseState::~PauseState()
{
}
//--------------------------------------------------------------
void PauseState::reset()
{
    setFinished(false);
    setNextState("");
    return;
}
//--------------------------------------------------------------
void PauseState::update()
{
}
//--------------------------------------------------------------
void PauseState::draw()
{
    ofSetColor(ofColor::black);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(ofColor::white);
    string text = "Game Paused. Press 'p' to resume.";
    ofDrawBitmapString(text, ofGetWidth() / 2 - 8 * text.length() / 2, ofGetHeight() / 2 - 11);
    return;
}
//--------------------------------------------------------------
void PauseState::keyPressed(int key)
{
    if (key == 'p')
    {
        setFinished(true);
        setNextState("GameState");
        return;
    }
}
