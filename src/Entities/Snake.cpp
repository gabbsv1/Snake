#include "Snake.h"
#include "ofMain.h"

Snake::Snake(int segmentSize, int boardSizeW, int boardSizeH, vector<StaticEntity *> staticEntities)
{
    body.push_back({8, 9});
    body.push_back({9, 9});
    body.push_back({10, 9});
    body.push_back({11, 9});
    this->direction = LEFT;
    this->crashed = false;
    this->segmentSize = segmentSize;
    this->boardSizeWidth = boardSizeW;
    this->boardSizeHeight = boardSizeH;
    this->staticEntities = staticEntities;
}

Snake::~Snake()
{
    // TODO Auto-generated destructor stub
}

void Snake::update()
{
    vector<int> oldHead = this->getHead();

    if (!godMode)
    {
        if (oldHead[0] == -1 || oldHead[0] == boardSizeWidth || oldHead[1] == -1 || oldHead[1] == boardSizeHeight)
        {
            crashed = true;
            return;
        }
        for (auto rock : staticEntities)
        {
            if (oldHead[0] == rock->getX() && oldHead[1] == rock->getY())
            {
                crashed = true;
                return;
            }
        }
    }
    else
    {
        if ((getHead()[0] == 0 && direction == LEFT) || (getHead()[1] == 0 && direction == UP) ||
            (getHead()[0] == boardSizeWidth - 1 && direction == RIGHT) ||
            (getHead()[1] == boardSizeHeight - 1 && direction == DOWN)) // checks if snake is headed toward border and stops it
        {
            changeDirection(NONE);
        }
        for (auto rock : staticEntities)
        {
            switch (direction)
            {
            case RIGHT:
                if (oldHead[0] + 1 == rock->getX() && oldHead[1] == rock->getY())
                {
                    changeDirection(NONE);
                }
                break;
            case LEFT:
                if (oldHead[0] - 1 == rock->getX() && oldHead[1] == rock->getY())
                {
                    changeDirection(NONE);
                }
                break;
            case UP:
                if (oldHead[0] == rock->getX() && oldHead[1] - 1 == rock->getY())
                {
                    changeDirection(NONE);
                }
                break;
            case DOWN:
                if (oldHead[0] == rock->getX() && oldHead[1] + 1 == rock->getY())
                {
                    changeDirection(NONE);
                }
                break;
            case NONE:
                break;
            }
        }
    }

    switch (this->direction)
    {
    case LEFT:
    {
        this->body[0][0] -= 1;
        break;
    }
    case DOWN:
    {
        this->body[0][1] += 1;
        break;
    }
    case RIGHT:
    {
        this->body[0][0] += 1;
        break;
    }
    case UP:
    {
        this->body[0][1] -= 1;
        break;
    }
    case NONE:
    {
        break;
    }
    }

    if (direction != NONE) // snake movement
    {
        int prevX = oldHead[0];
        int prevY = oldHead[1];
        for (unsigned int i = 1; i < this->body.size(); i++)
        {
            int currX = this->body[i][0];
            int currY = this->body[i][1];
            this->body[i][0] = prevX;
            this->body[i][1] = prevY;
            prevX = currX;
            prevY = currY;
        }
    }

    if (!godMode)
        checkSelfCrash();
}

void Snake::loseFat()
{
    if (body.size() > 2)
        body.pop_back();
}

void Snake::draw()
{
    for (unsigned int i = 0; i < body.size(); i++)
    {
        int curX = this->body[i][0];
        int curY = this->body[i][1];
        if (i == 0)
        {
            ofSetColor(ofColor::yellow);
        }
        else
        {
            ofSetColor(ofColor::green);
        }
        ofDrawRectangle(curX * segmentSize, curY * segmentSize, segmentSize, segmentSize);
    }
}

void Snake::changeDirection(Direction d)
{

    if (this->direction == LEFT and d == RIGHT)
        return;
    if (this->direction == RIGHT and d == LEFT)
        return;
    if (this->direction == UP and d == DOWN)
        return;
    if (this->direction == DOWN and d == UP)
        return;

    this->direction = d;
}

void Snake::checkSelfCrash()
{
    std::vector<std::vector<int>> snake = this->body;
    vector<int> head = snake[0];
    for (unsigned int i = 1; i < snake.size(); i++)
    {
        if (head[0] == snake[i][0] and head[1] == snake[i][1])
        {
            crashed = true;
            return;
        }
    }
}

void Snake::grow()
{
    vector<int> newSegment = this->getTail();
    this->body.push_back(newSegment);
}