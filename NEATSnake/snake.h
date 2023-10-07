#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "food.h"
#include "util.h"

Intersection intersection[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

class Snake;

enum dir
{
    left,
    up,
    right,
    down
};

extern int BlockSize;
extern int SnakeCanvas;
extern int SnakeCanvasW;
extern int SnakeCanvasH;
extern int SnakeCanvasX;
extern int SnakeCanvasY;
extern int GridSize;

class Snake
{

public:
    pos arr[625];
    int direction;
    int length;
    int frames;

    // Vision Variables
    int rightDisFood = 0;
    int rightDisWall = 0;
    int rightDisTail = 0;

    int leftDisFood = 0;
    int leftDisWall = 0;
    int leftDisTail = 0;

    int upDisFood = 0;
    int upDisWall = 0;
    int upDisTail = 0;

    int downDisFood = 0;
    int downDisWall = 0;
    int downDisTail = 0;

    int rightUpDisFood = 0;
    int rightUpDisWall = 0;
    int rightUpDisTail = 0;

    int rightDownDisFood = 0;
    int rightDownDisWall = 0;
    int rightDownDisTail = 0;

    int leftUpDisFood = 0;
    int leftUpDisWall = 0;
    int leftUpDisTail = 0;

    int leftDownDisFood = 0;
    int leftDownDisWall = 0;
    int leftDownDisTail = 0;

    int dead = 0;
    int score = 0;

    int foodEaten = 0;

    Food food;
    SDL_Rect snake;

    Snake()
    {
        arr[0].x = BlockSize;
        arr[0].y = BlockSize;
        length = 2;
        direction = right;
    }

    void drawSnake(SDL_Renderer *);
    void drawVision(SDL_Renderer *);
    void appendSnake();
    void changeDir(int);
    void init();
    void appendVision();
    int update();
    int getPosx();
    int getPosy();
    int getlength();
    int getBodyPosx();
    int getBodyPosy();
    int foodPosy();
    int foodPosx();
    int getDis(int, int, int, int);
    bool towardsFood(int, int, int, int);
};

void Snake::init()
{
    arr[0].x = BlockSize + (SnakeCanvasW - (SnakeCanvas / 2));
    arr[0].y = BlockSize + (SnakeCanvasH - (SnakeCanvas / 2));
    length = 2;
    dead = 0;
    score = 0;
    foodEaten = 0;
    direction = right;

    food.generate(getBodyPosx(), getBodyPosy());
}

void Snake::drawSnake(SDL_Renderer *rend)
{

    for (int i = 0; i < length; i++)
    {
        SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);

        snake.x = arr[i].x;
        snake.y = arr[i].y;
        snake.w = BlockSize - 2;
        snake.h = BlockSize - 2;

        SDL_RenderFillRect(rend, &snake);
    }

    food.draw(rend);
}

void Snake::appendSnake()
{
    if (length < GridSize * GridSize)
    {
        score += 10;
        foodEaten++;
        length++;
    }
}

void Snake::changeDir(int newdir)
{
    if (newdir == left || newdir == right)
    {
        if (direction == up || direction == down)
        {
            direction = newdir;
        }
    }
    else if (newdir == up || newdir == down)
    {
        if (direction == left || direction == right)
        {
            direction = newdir;
        }
    }
}

void Snake::appendVision()
{
    int headx = arr[0].x + (BlockSize / 2);
    int heady = arr[0].y + (BlockSize / 2);

    int SnakeCanvasTotalX = 1000;
    int SnakeCanvasLeftX = 500;
    int SnakeCanvasTopY = 0;
    int SnakeCanvasBottomY = 500;
    // VisionRight
    // Vision Sees food
    if (SDL_IntersectRectAndLine(&food.food, &headx, &heady, &SnakeCanvasTotalX, &heady))
    {
        rightDisFood = foodPosx() - headx;
    }
    // Vision Sees Walls
    rightDisWall = SnakeCanvasTotalX - headx;
    // Vision Sees Tail
    if (SDL_IntersectRectAndLine(&snake, &headx, &heady, &SnakeCanvasTotalX, &heady))
    {
        rightDisTail = snake.x - headx;
    }
    // VisionLeft
    // Vision Sees Food
    if (SDL_IntersectRectAndLine(&food.food, &headx, &heady, &SnakeCanvasLeftX, &heady))
    {
        leftDisFood = headx - foodPosx();
    }
    // Vision Sees Walls
    leftDisWall = headx - SnakeCanvasLeftX;
    // Vision Sees Tail
    if (SDL_IntersectRectAndLine(&snake, &headx, &heady, &SnakeCanvasLeftX, &heady))
    {
        leftDisTail = headx - snake.x;
    }
    // VisionUp
    // Vision Sees Food
    if (SDL_IntersectRectAndLine(&food.food, &headx, &heady, &headx, &SnakeCanvasTopY))
    {
        upDisFood = heady - foodPosy();
    }
    // Vision Sees Walls
    upDisWall = heady - SnakeCanvasTopY;
    // Vision Sees Tail
    if (SDL_IntersectRectAndLine(&snake, &headx, &heady, &headx, &SnakeCanvasTopY))
    {
        upDisTail = heady - snake.y;
    }
    // VisionDown
    // Vision Sees Food
    if (SDL_IntersectRectAndLine(&food.food, &headx, &heady, &headx, &SnakeCanvasBottomY))
    {
        downDisFood = foodPosy() - heady;
    }
    // Vision Sees Walls
    downDisWall = SnakeCanvasBottomY - heady;
    // Vision Sees Tail
    if (SDL_IntersectRectAndLine(&snake, &headx, &heady, &headx, &SnakeCanvasBottomY))
    {
        downDisTail = snake.y - heady;
    }
}

void Snake::drawVision(SDL_Renderer *rend)
{
    int headx = arr[0].x + (BlockSize / 2);
    int heady = arr[0].y + (BlockSize / 2);

    Vector2d head = {headx, heady};

    int SnakeCanvasTotalX = 1000;
    int SnakeCanvasLeftX = 500;
    int SnakeCanvasTopY = 0;
    int SnakeCanvasBottomY = 500;

    SDL_SetRenderDrawColor(rend, 255, 69, 0, 255);

    Vector2d end_segment[] = {{SnakeCanvasTotalX, heady},                              // Right
                              {SnakeCanvasLeftX, heady},                               // Left
                              {headx, SnakeCanvasTopY},                                // Up
                              {headx, SnakeCanvasBottomY},                             // Down
                              {headx + 500, heady - 500},                              // Up Right
                              {headx + SnakeCanvasTotalX, heady + SnakeCanvasBottomY}, // Down Right
                              {headx - 500, heady - 500},                              // Up Left
                              {headx - SnakeCanvasLeftX, heady + SnakeCanvasBottomY}}; // Down Left

    for (int i = 0; i < sizeof(end_segment) / sizeof(end_segment[0]); i++)
    {
        intersection[i] = findIntersection(head, end_segment[i], food.foodPoint().a, food.foodPoint().b, food.foodPoint().c, food.foodPoint().d);

        SDL_RenderDrawLine(rend, headx, heady, end_segment[i].x, end_segment[i].y);

        if (intersection[i].p.x != 0 && intersection[i].p.y != 0)
        {
            SDL_RenderFillCircle(rend, intersection[i].p.x, intersection[i].p.y, 5);
        }
    }
}

int Snake::update()
{

    for (int i = 1; i < length; i++)
    {
        if (arr[0].x == arr[i].x && arr[0].y == arr[i].y)
        {
            dead = 1;
            return 0;
        }
    }

    for (int i = length; i >= 0; i--)
    {
        if (i == 0)
        {
            arr[1].x = arr[0].x;
            arr[1].y = arr[0].y;
        }
        else
        {
            arr[i].x = arr[i - 1].x;
            arr[i].y = arr[i - 1].y;
        }
    }

    if (direction == left)
    {
        arr[0].x -= BlockSize;
        if (arr[0].x == SnakeCanvasX - BlockSize)
        {
            // arr[0].x = SnakeCanvas;
            dead = 1;
            return 0;
        }
    }

    if (direction == right)
    {
        arr[0].x += BlockSize;
        if (arr[0].x == SnakeCanvasW)
        {
            // arr[0].x = 0;
            dead = 1;
            return 0;
        }
    }

    if (direction == up)
    {
        arr[0].y -= BlockSize;
        if (arr[0].y == SnakeCanvasY - BlockSize)
        {
            // arr[0].y = SnakeCanvas;
            dead = 1;
            return 0;
        }
    }

    if (direction == down)
    {
        arr[0].y += BlockSize;
        if (arr[0].y == SnakeCanvasH)
        {
            // arr[0].y = 0;
            dead = 1;
            return 0;
        }
    }

    if (food.update(getPosx(), getPosy()))
    {
        food.generate(getBodyPosx(), getBodyPosy());
        appendSnake();
        frames = 0;
    }

    frames++;

    if (frames >= GridSize * GridSize && !food.update(getPosx(), getPosy()))
    {
        dead = 1;
        frames = 0;
    }

    if (towardsFood(arr[0].x, arr[0].y, food.getFoodPosx(), food.getFoodPosy()))
    {
        score += 1;
    }
    else
    {
        score -= (float)0.2;
    }

    if (dead)
    {
        frames = 0;
        return 0;
    }

    if (score < 0)
    {
        score = 0;
    }

    dead = 0;

    return 1;
}

int Snake::getPosx()
{
    return arr[0].x;
}

int Snake::getPosy()
{
    return arr[0].y;
}

int Snake::getlength()
{
    return length;
}

int Snake::getBodyPosx()
{
    for (int i = 0; i < length; i++)
    {
        return arr[i].x;
    }
    return 1;
}

int Snake::getBodyPosy()
{
    for (int i = 0; i < length; i++)
    {
        return arr[i].y;
    }
    return 1;
}

int Snake::foodPosx()
{
    return food.getFoodPosx();
}

int Snake::foodPosy()
{
    return food.getFoodPosy();
}

int Snake::getDis(int x1, int y1, int x2, int y2)
{
    // Distance Formala!!
    return (sqrt(abs(x2 - x1) * abs(x2 - x1) + abs(y2 - y1) * abs(y2 - y1)));
}

bool Snake::towardsFood(int x1, int y1, int x2, int y2)
{
    int toPoint = getDis(x1, y1, x2, y2);
    if (toPoint == toPoint--)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif