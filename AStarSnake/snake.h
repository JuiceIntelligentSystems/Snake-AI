#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define BLOCK_SIZE 25
#define GRID_ROWS WINDOW_HEIGHT / BLOCK_SIZE
#define GRID_COLS WINDOW_WIDTH / BLOCK_SIZE

#define ASTAR_GRID_ROWS GRID_ROWS
#define ASTAR_GRID_COLS GRID_COLS 

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

    pos snakeHead_relative_to_grid;
    pos snakeTail_relative_to_grid[GRID_ROWS * GRID_COLS];

    pos foodPos_relative_to_grid;

    int tail_wall_grid[ASTAR_GRID_ROWS][ASTAR_GRID_COLS] = {0};

    int length;
    int direction;
    int frames;

    bool dead = false;
    int score = 0;

    int path_num = 1;

    SDL_Rect snakeRect;

    void init();
    void draw(SDL_Renderer *);
    void append();
    void changeDir(int);
    void loadMatrix(pos, bool);
    void clearMatrix();
    void followPath(int path[ASTAR_GRID_ROWS][ASTAR_GRID_COLS]);
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
        if (snakeBlock[0].y < -1)
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

void Snake::loadMatrix(pos foodPos, bool printMatrix)
{
    // Load Snake Head Coordinates
    snakeHead_relative_to_grid.x = snakeBlock[0].x / (GRID_COLS * 0.77);
    snakeHead_relative_to_grid.y = snakeBlock[0].y / (GRID_ROWS * 0.77);

    // Load Food Coordinates
    foodPos_relative_to_grid.x = foodPos.x / (GRID_COLS * 0.77);
    foodPos_relative_to_grid.y = foodPos.y / (GRID_ROWS * 0.77);

    // Load Snake Tail Coordinates
    for (int i = 0; i < length; i++)
    {
        snakeTail_relative_to_grid[i].x = snakeBlock[i+1].x / (GRID_COLS * 0.77);
        snakeTail_relative_to_grid[i].y = snakeBlock[i+1].y / (GRID_ROWS * 0.77);

        tail_wall_grid[snakeTail_relative_to_grid[i].y][snakeTail_relative_to_grid[i].x] = 1;
    }

    // Display Matrix
    if (printMatrix)
    {
        for (int i = 0; i < GRID_ROWS; i++)
        {
            for (int j = 0; j < GRID_COLS; j++)
            {
                std::cout << tail_wall_grid[i][j];
            }
            std::cout << std::endl;
        }
    }

    // Clear the Matrix for the Next Run
    // NO U STUPID DONT DO THAT OTHERWISE THE ALGRITHM WONT RECONGNIZE THE WALLS
}

void Snake::clearMatrix()
{
    // I Spent too long on this part
    // Clear the matrix AFTER astar algorithm
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
            tail_wall_grid[i][j] = 0;
        }
    }
}

void Snake::followPath(int path[ASTAR_GRID_ROWS][ASTAR_GRID_COLS])
{
    if (path[snakeHead_relative_to_grid.y + 1][snakeHead_relative_to_grid.x] == path_num)
    {
        changeDir(down);
        path_num++;
    }
    if (path[snakeHead_relative_to_grid.y - 1][snakeHead_relative_to_grid.x] == path_num)
    {
        changeDir(up);
        path_num++;
    }
    if (path[snakeHead_relative_to_grid.y][snakeHead_relative_to_grid.x + 1] == path_num)
    {
        changeDir(right);
        path_num++;
    }
    if (path[snakeHead_relative_to_grid.y][snakeHead_relative_to_grid.x - 1] == path_num)
    {
        changeDir(left);
        path_num++;
    }
}

#endif