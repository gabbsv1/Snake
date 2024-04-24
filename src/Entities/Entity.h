#pragma once
#include "ofMain.h"

class Entity {
  protected:
    int x, y, width, height, health;
    ofImage sprite;
   

  public:
    Entity(int x, int y, int width, int height, ofImage sprite);
    void update();
    void draw();
    int getX() { return x; }
    int getY() { return y; }
    int getWidth() { return width; }
    int getHeight() { return height; }
};