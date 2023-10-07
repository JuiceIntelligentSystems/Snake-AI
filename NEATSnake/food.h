#ifndef FOOD_H
#define FOOD_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <ctime>
#include "util.h"

extern int BlockSize;
extern int GridSize;
extern int SnakeCanvasX;
extern int SnakeCanvasY;

struct pos
{
    int x, y;
    pos()
    {
        x = -50;
        y = 0;
    }
};

struct foodp
{
    Vector2d a, b;
    Vector2d c, d;
};

class Food
{

private:
    pos foodPos;

public:
    SDL_Rect food;
    void draw(SDL_Renderer *);
    void generate(int, int);
    bool update(int, int);
    int getFoodPosx();
    int getFoodPosy();
    foodp foodPoint();
};

void Food::draw(SDL_Renderer *rend)
{
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);

    food.x = foodPos.x;
    food.y = foodPos.y;
    food.w = BlockSize - 2;
    food.h = BlockSize - 2;

    SDL_RenderFillRect(rend, &food);
}

void Food::generate(int snakeHeadx, int snakeHeady)
{
    // srand(time(0));
    foodPos.x = SnakeCanvasX + (BlockSize * (rand() % GridSize));
    // srand(time(0));
    foodPos.y = SnakeCanvasY + (BlockSize * (rand() % GridSize));
    if (foodPos.x == snakeHeadx && foodPos.y == snakeHeady)
    {
        generate(snakeHeadx, snakeHeady);
    }
}

bool Food::update(int snakeHeadx, int snakeHeady)
{
    if (foodPos.x == snakeHeadx && foodPos.y == snakeHeady)
    {
        return true;
    }
    return false;
}

int Food::getFoodPosx()
{
    return foodPos.x;
}

int Food::getFoodPosy()
{
    return foodPos.y;
}

foodp Food::foodPoint()
{
    Vector2d foodPoint_a = {food.x, food.y};
    Vector2d foodPoint_b = {food.x + food.w, food.y};
    Vector2d foodPoint_c = {food.x, food.y + food.h};
    Vector2d foodPoint_d = {food.x + food.w, food.y + food.h};

    return {foodPoint_a, foodPoint_b, foodPoint_c, foodPoint_d};
}

#endif