#include "Entity.h"

Entity::Entity(int x, int y, int width, int height, ofImage sprite){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->sprite = sprite;
}


void Entity::draw(){
    ofSetColor (255,255,255);
    sprite.draw(x, y, width, height);
}