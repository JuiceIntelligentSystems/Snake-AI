#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern int SnakeCanvasX;
extern int SnakeCanvasY;
extern int SnakeCanvasW;
extern int SnakeCanvasH;
extern int SnakeCanvas;
extern int windowX;
extern int windowY;

char *scoreText = (char *)malloc(sizeof(scoreText));
char *generationText = (char *)malloc(sizeof(scoreText));
char *bestscoreText = (char *)malloc(sizeof(bestscoreText));

TTF_Font *Sans;

SDL_Color White;

SDL_Rect scoreRect;
SDL_Rect genRect;
SDL_Rect bestscoreRect;

SDL_Surface *scoreMessage;
SDL_Texture *scoreTexture;
SDL_Surface *genMessage;
SDL_Texture *genTexture;
SDL_Surface *bestscoreMessage;
SDL_Texture *bestscoreTexture;

void initTTF()
{
    if (TTF_Init() < 0)
    {
        printf("Failed to init TTF: %d\n", TTF_GetError());
    }

    Sans = TTF_OpenFont("OpenSans-Regular.ttf", 30);

    White = {255, 255, 255};

    scoreRect.x = SnakeCanvasX - 10;
    scoreRect.y = SnakeCanvasH;
    scoreRect.w = 140;
    scoreRect.h = 60;

    genRect.x = scoreRect.x + 300;
    genRect.y = SnakeCanvasH;
    genRect.w = 140;
    genRect.h = 60;

    bestscoreRect.x = SnakeCanvasX - 10;
    bestscoreRect.y = scoreRect.y + scoreRect.h - 10;
    bestscoreRect.w = 140;
    bestscoreRect.h = 60;
}

void initStats(SDL_Renderer *rend, int score, int gen, int bestscore)
{
    sprintf(scoreText, "Score: %d", score);
    sprintf(generationText, "Gen: %d", gen);
    sprintf(bestscoreText, "Best: %d", bestscore);

    scoreMessage = TTF_RenderText_Solid(Sans, scoreText, White);
    scoreTexture = SDL_CreateTextureFromSurface(rend, scoreMessage);
    genMessage = TTF_RenderText_Solid(Sans, generationText, White);
    genTexture = SDL_CreateTextureFromSurface(rend, genMessage);
    bestscoreMessage = TTF_RenderText_Solid(Sans, bestscoreText, White);
    bestscoreTexture = SDL_CreateTextureFromSurface(rend, bestscoreMessage);
}

void drawStats(SDL_Renderer *rend)
{
    SDL_RenderCopy(rend, scoreTexture, NULL, &scoreRect);
    SDL_RenderCopy(rend, genTexture, NULL, &genRect);
    SDL_RenderCopy(rend, bestscoreTexture, NULL, &bestscoreRect);

    SDL_FreeSurface(scoreMessage);
    SDL_DestroyTexture(scoreTexture);
    SDL_FreeSurface(genMessage);
    SDL_DestroyTexture(genTexture);
    SDL_FreeSurface(bestscoreMessage);
    SDL_DestroyTexture(bestscoreTexture);
}

void drawBoundries(SDL_Renderer *rend)
{
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

    SDL_Rect boundryX;
    SDL_Rect boundryY;

    boundryX.x = SnakeCanvasX - 12;
    boundryX.y = SnakeCanvasY;
    boundryX.w = 10;
    boundryX.h = SnakeCanvasH + 10;

    boundryY.x = SnakeCanvasX - 2;
    boundryY.y = SnakeCanvasH;
    boundryY.w = SnakeCanvasW;
    boundryY.h = 10;

    SDL_RenderFillRect(rend, &boundryX);
    SDL_RenderFillRect(rend, &boundryY);
}

#endif