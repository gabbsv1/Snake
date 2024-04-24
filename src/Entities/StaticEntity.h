#pragma once

#include "Entity.h"

class StaticEntity : public Entity {

    public:
        StaticEntity(int x, int y, int width, int height, ofImage sprite); //Constructor
        void draw();
};