#include <iostream>
#include <ctime>
#include <SDL2/SDL.h>
#include "snake.h"
#include "food.h"
#include "Astar.h"

int main(int argc, char *argv[])
{
    srand((unsigned)time((time_t *)NULL));

    Snake snake;
    Food food;
    Astar astar;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Error Initializing SDL: " << SDL_GetError();
        return -1;
    }

    SDL_Window *win = SDL_CreateWindow("A* Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    Uint32 renderflags = SDL_RENDERER_ACCELERATED;

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, renderflags);

    snake.init();
    food.generate(snake.snakeBlock, snake.length);

    astar.displayPath = true;
    snake.loadMatrix(food.foodPos, false);

    Pair src = std::make_pair(snake.snakeHead_relative_to_grid.y, snake.snakeHead_relative_to_grid.x);
    Pair dest = std::make_pair(snake.foodPos_relative_to_grid.y, snake.foodPos_relative_to_grid.x);

    astar.aStarSearch(snake.tail_wall_grid, src, dest, false);

    int speed = 11;
    bool close = false;
    while (!close)
    {
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                close = true;
                break;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    close = true;
                    break;
                    /*
                    case SDLK_UP:
                        snake.changeDir(up);
                        break;
                    case SDLK_DOWN:
                        snake.changeDir(down);
                        break;
                    case SDLK_LEFT:
                        snake.changeDir(left);
                        break;
                    case SDLK_RIGHT:
                        snake.changeDir(right);
                        break;
                    */
                }
            }
        }
        if (!snake.dead)
        {
            snake.loadMatrix(food.foodPos, false);
            snake.followPath(astar.aStar_trace_path);

            snake.update();

            if (food.update(snake.snakeBlock[0].x, snake.snakeBlock[0].y))
            {
                snake.path_num = 1;

                snake.append();
                food.generate(snake.snakeBlock, snake.length);

                astar.clearPath();
                snake.loadMatrix(food.foodPos, false);

                Pair src = std::make_pair(snake.snakeHead_relative_to_grid.y, snake.snakeHead_relative_to_grid.x);
                Pair dest = std::make_pair(snake.foodPos_relative_to_grid.y, snake.foodPos_relative_to_grid.x);

                astar.aStarSearch(snake.tail_wall_grid, src, dest, false);

                std::cout << "Score: " << snake.score << std::endl;
            }
            snake.clearMatrix();
        }
        snake.draw(rend);
        food.draw(rend);

        SDL_RenderPresent(rend);
        SDL_Delay(speed);
    }
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}