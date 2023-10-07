#ifndef FOOD_H
#define FOOD_H

#include <iostream>
#include <SDL2/SDL.h>

class Food
{
private:
    SDL_Rect food;

public:
    pos foodPos;

    void draw(SDL_Renderer *);
    void generate(pos *, int);
    bool update(int, int);
    int getFoodPosx();
    int getFoodPosy();
};

void Food::draw(SDL_Renderer *rend)
{
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);

    food.w = BLOCK_SIZE - 2;
    food.h = BLOCK_SIZE - 2;
    food.x = foodPos.x + 2;
    food.y = foodPos.y + 2;

    SDL_RenderFillRect(rend, &food);
}

void Food::generate(pos snakeBlock[GRID_ROWS * GRID_COLS], int length)
{
    foodPos.x = BLOCK_SIZE * (rand() % GRID_COLS);
    foodPos.y = BLOCK_SIZE * (rand() % GRID_ROWS);

    for (int i = 0; i < length; i++)
    {
        if (snakeBlock[i].x == foodPos.x && snakeBlock[i].y == foodPos.y)
        {
            generate(snakeBlock, length);
        }
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

#endif