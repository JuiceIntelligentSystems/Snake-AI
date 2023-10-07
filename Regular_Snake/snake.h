#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define BLOCK_SIZE 25
#define GRID_ROWS WINDOW_HEIGHT / BLOCK_SIZE
#define GRID_COLS WINDOW_WIDTH / BLOCK_SIZE

enum dir
{
    up,
    down,
    left,
    right
};

struct pos
{
    int x, y;
};

class Snake
{
public:
    pos snakeBlock[GRID_ROWS * GRID_COLS];
    int length;
    int direction;
    int frames;

    bool dead = false;
    int score = 0;

    SDL_Rect snakeRect;

    void init();
    void draw(SDL_Renderer *);
    void append();
    void changeDir(int);
    int update();
};

void Snake::init()
{
    snakeBlock[0].x = BLOCK_SIZE + (WINDOW_WIDTH / 2);
    snakeBlock[0].y = BLOCK_SIZE + (WINDOW_HEIGHT / 2);
    length = 2;
    direction = right;
    score = 0;
    dead = false;
}

void Snake::draw(SDL_Renderer *rend)
{
    for (int i = 0; i < length; i++)
    {
        SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);

        snakeRect.w = BLOCK_SIZE - 2;
        snakeRect.h = BLOCK_SIZE - 2;
        snakeRect.x = snakeBlock[i].x + 2;
        snakeRect.y = snakeBlock[i].y + 2;

        if (snakeBlock[i + 1].x == snakeBlock[i].x + BLOCK_SIZE && snakeBlock[i + 1].y == snakeBlock[i].y)
        {
            snakeRect.w = BLOCK_SIZE;
        }
        if (snakeBlock[i + 1].x == snakeBlock[i].x - BLOCK_SIZE && snakeBlock[i + 1].y == snakeBlock[i].y)
        {
            snakeRect.x = snakeBlock[i].x - 2;
            snakeRect.w = BLOCK_SIZE + 2;
        }
        if (snakeBlock[i + 1].y == snakeBlock[i].y + BLOCK_SIZE && snakeBlock[i + 1].x == snakeBlock[i].x)
        {
            snakeRect.h = BLOCK_SIZE;
        }
        if (snakeBlock[i + 1].y == snakeBlock[i].y - BLOCK_SIZE && snakeBlock[i + 1].x == snakeBlock[i].x)
        {
            snakeRect.y = snakeBlock[i].y - 2;
            snakeRect.h = BLOCK_SIZE + 2;
        }

        SDL_RenderFillRect(rend, &snakeRect);
    }
}

void Snake::append()
{
    if (length < GRID_ROWS * GRID_COLS)
    {
        score++;
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
    if (newdir == up || newdir == down)
    {
        if (direction == left || direction == right)
        {
            direction = newdir;
        }
    }
}

int Snake::update()
{
    for (int i = 1; i < length; i++)
    {
        if (snakeBlock[0].x == snakeBlock[i].x && snakeBlock[0].y == snakeBlock[i].y)
        {
            dead = 1;
            return 0;
        }
    }

    for (int i = length; i >= 0; i--)
    {
        if (i == 0)
        {
            snakeBlock[1].x = snakeBlock[0].x;
            snakeBlock[1].y = snakeBlock[0].y;
        }
        else
        {
            snakeBlock[i].x = snakeBlock[i - 1].x;
            snakeBlock[i].y = snakeBlock[i - 1].y;
        }
    }

    if (direction == left)
    {
        snakeBlock[0].x -= BLOCK_SIZE;
        if (snakeBlock[0].x < 0)
        {
            snakeBlock[0].x = 0;
            dead = 1;
            return 0;
        }
    }

    if (direction == right)
    {
        snakeBlock[0].x += BLOCK_SIZE;
        if (snakeBlock[0].x > WINDOW_WIDTH - BLOCK_SIZE)
        {
            snakeBlock[0].x = WINDOW_WIDTH - BLOCK_SIZE;
            dead = 1;
            return 0;
        }
    }

    if (direction == up)
    {
        snakeBlock[0].y -= BLOCK_SIZE;
        if (snakeBlock[0].y < 0)
        {
            snakeBlock[0].y = 0;
            dead = 1;
            return 0;
        }
    }

    if (direction == down)
    {
        snakeBlock[0].y += BLOCK_SIZE;
        if (snakeBlock[0].y > WINDOW_HEIGHT - BLOCK_SIZE)
        {
            snakeBlock[0].y = WINDOW_HEIGHT - BLOCK_SIZE;
            dead = 1;
            return 0;
        }
    }
    dead = false;
    return 1;
}

#endif