#pragma once
#include <vector>
#include <StaticEntity.h>

enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
};

class Snake
{
private:
    int boardSizeWidth, boardSizeHeight;
    bool crashed;

public:
    std::vector<std::vector<int>> body;
    Direction direction;
    int segmentSize;
    vector<StaticEntity *> staticEntities;
    bool godMode = false;

    Snake(int segmentSize, int boardSizeW, int boardSizeH, vector<StaticEntity *> staticEntities);
    ~Snake();

    void update();
    void draw();
    void changeDirection(Direction d);
    void checkSelfCrash();
    void grow();
    void loseFat();
    bool isCrashed()
    {
        return this->crashed;
    }

    std::vector<int> getHead()
    {
        return this->body[0];
    }

    std::vector<int> getTail()
    {
        return this->body[this->body.size() - 1];
    }

    std::vector<std::vector<int>> getBody()
    {
        return this->body;
    }
    void setCrashed(bool crashed)
    {
        this->crashed = crashed;
    }
};
