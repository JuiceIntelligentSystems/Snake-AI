#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "neat.h"
#include "snake.h"
#include "think.h"
#include "window.h"

int POPULATION = 1;

int windowX = 1000;
int windowY = 800;

int SnakeCanvas = 500;
int SnakeCanvasX = 1000 - SnakeCanvas;
int SnakeCanvasY = 0;
int SnakeCanvasW = SnakeCanvasX + SnakeCanvas;
int SnakeCanvasH = SnakeCanvas;

int BlockSize = 25;
int GridSize = SnakeCanvas / BlockSize;

int gen;
int speed = 100;

int main()
{
    srand((unsigned)time((time_t *)NULL));

    Snake *snake = (Snake *)malloc(sizeof(Snake) * POPULATION);
    network **brain = (network **)malloc(sizeof(network *) * POPULATION);

    int nmap[] = {12, 8, 4};

    for (int i = 0; i < POPULATION; i++)
    {
        brain[i] = confNet(3, nmap);
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Problem loading SDL: %d\n", SDL_GetError());
    }

    SDL_Window *win = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX, windowY, 0);
    // SDL_Window *winNet = SDL_CreateWindow("Network", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, 0);

    Uint32 renderflags = SDL_RENDERER_ACCELERATED;
    // Uint32 renderflags1 = SDL_RENDERER_ACCELERATED;

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, renderflags);
    // SDL_Renderer *rendNet = SDL_CreateRenderer(winNet, -1, renderflags1);

    initTTF();

    int length, count = 0;
    bool playing = true;
    int running = 1;
    int dead = 0;
    int best = 0;
    int bests = 0;
    int foodate = 0;
    int index = 0;
    char lengtharr[3];
    bool drawall = false;
    bool draw_vision = false;

    for (int i = 0; i < POPULATION; i++)
    {
        snake[i].init();
    }

    while (running)
    {
        dead = 0;

        while (!dead)
        {
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderClear(rend);

            initStats(rend, foodate, gen, bests);

            best = 0;

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
                if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    /*case SDLK_LEFT:
                        snake.changeDir(left);
                        break;
                    case SDLK_UP:
                        snake.changeDir(up);
                        break;
                    case SDLK_RIGHT:
                        snake.changeDir(right);
                        break;
                    case SDLK_DOWN:
                        snake.changeDir(down);
                        break; */
                    case SDLK_r:
                        draw_vision = true;
                        break;
                    case SDLK_UP:
                        speed--;
                        if (speed < 5)
                        {
                            speed = 5;
                        }
                        break;
                    case SDLK_DOWN:
                        speed++;
                        if (speed > 150)
                        {
                            speed = 150;
                        }
                        break;
                    case SDLK_g:
                        dead = 1;
                        for (int i = 0; i < POPULATION; i++)
                        {
                            // snake[0].score = 0;
                            snake[i].dead = 1;
                        }
                        break;
                    case SDLK_d:
                        drawall = true;
                        break;
                    case SDLK_e:
                        drawall = false;
                        break;
                    }
                }
            }

            think(brain, snake, snake->foodPosx(), snake->foodPosy());
            dead = 1;

            for (int i = 0; i < POPULATION; i++)
            {
                if (!snake[i].dead)
                {
                    dead = 0;

                    if (drawall)
                    {
                        snake[index].drawSnake(rend);
                        if (draw_vision) {
                            snake[index].drawVision(rend);
                        }
                    }
                    else
                    {
                        snake[i].drawSnake(rend);
                        if (draw_vision) {
                            snake[i].drawVision(rend);
                        }
                    }

                    if (snake[i].score > best)
                    {
                        best = snake[i].score;
                        foodate = snake[i].foodEaten;
                        index = i;
                    }
                }
            }

            if (foodate > bests)
            {
                bests = foodate;
            }

            drawNet(brain[index], rend, 0, 0, windowX - SnakeCanvasX - 10, windowY, 5);
            drawBoundries(rend);
            drawStats(rend);

            SDL_RenderPresent(rend);
            // SDL_RenderPresent(rendNet);
            SDL_Delay(speed);
        }

        if (index != 0)
        {
            free(brain[0]);
            brain[0] = copyNet(brain[index]);
        }

        snake[0].init();

        for (int i = 1; i < POPULATION; i++)
        {

            int r = rand() % 2;

            if (r == 1)
            {
                if (rand() % 100 - 60 > 0)
                {
                    r = 0;
                }
            }

            free(brain[i]);
            brain[i] = copyNet(brain[0]);
            mutateNet(brain[i], r, 2);

            snake[i].init();
        }
        gen++;
    }

    // SDL_DestroyRenderer(rendNet);
    SDL_DestroyRenderer(rend);
    // SDL_DestroyWindow(winNet);
    SDL_DestroyWindow(win);
    return 0;
}