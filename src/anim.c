#include "anim.h"

static SDL_Texture *runtext[17] = { NULL };
static SDL_Texture *rruntext[17] = { NULL };
static SDL_Texture *throwtext[28] = { NULL };
static SDL_Texture *rthrowtext[28] = { NULL };

void parserun(struct game *game)
{
    char buf[64] = "./res/running/COURSE00";
    char *ptr = buf;
    while (*ptr)
        ptr++;
    for (int i = 0; i <= 9; i++)
    {
        *ptr = '0';
        sprintf(ptr + 1, "%d.png", i);
        SDL_Surface *surf = IMG_Load(buf);
        runtext[i] = SDL_CreateTextureFromSurface(game->render, surf);
        SDL_FreeSurface(surf);
    }
    for (int i = 0; i <= 6; i++)
    {
        *ptr = '1';
        sprintf(ptr + 1, "%d.png", i);
        SDL_Surface *surf = IMG_Load(buf);
        runtext[i + 10] = SDL_CreateTextureFromSurface(game->render, surf);
        SDL_FreeSurface(surf);
    }
    game->player.runtext = runtext;
}

void parserrun(struct game *game)
{
    char buf[64] = "./res/running/RCOURSE00";
    char *ptr = buf;
    while (*ptr)
        ptr++;
    for (int i = 0; i <= 9; i++)
    {
        *ptr = '0';
        sprintf(ptr + 1, "%d.png", i);
        SDL_Surface *surf = IMG_Load(buf);
        rruntext[i] = SDL_CreateTextureFromSurface(game->render, surf);
        SDL_FreeSurface(surf);
    }
    for (int i = 0; i <= 6 ; i++)
    {
        *ptr = '1';
        sprintf(ptr + 1, "%d.png", i);
        SDL_Surface *surf = IMG_Load(buf);
        rruntext[i + 10] = SDL_CreateTextureFromSurface(game->render, surf);
        SDL_FreeSurface(surf);
    }
    game->player.rruntext = rruntext;
}

void rparsethrow(struct game *game)
{
    char buf[64] = "./res/throw/RLANCER00";
    char *ptr = buf;
    while (*ptr)
        ptr++;
    for (int i = 0; i < 28; i++)
    {
        sprintf(ptr, "%02d.png", i);
        SDL_Surface *surf = IMG_Load(buf);
        rthrowtext[i] = SDL_CreateTextureFromSurface(game->render, surf);
        SDL_FreeSurface(surf);
    }
    game->player.rthrowtext = rthrowtext;
}

void parsethrow(struct game *game)
{
    char buf[64] = "./res/throw/LANCER00";
    char *ptr = buf;
    while (*ptr)
        ptr++;
    for (int i = 0; i < 28; i++)
    {
        sprintf(ptr, "%02d.png", i);
        SDL_Surface *surf = IMG_Load(buf);
        throwtext[i] = SDL_CreateTextureFromSurface(game->render, surf);
        SDL_FreeSurface(surf);
    }
    game->player.throwtext = throwtext;
}

void anim_destroy(void)
{
    for (int i = 0; i < RUN_SIZE; i++)
    {
        SDL_DestroyTexture(runtext[i]);
        SDL_DestroyTexture(rruntext[i]);
    }
    for (int i = 0; i < 28; i++)
    {
        SDL_DestroyTexture(throwtext[i]);
        SDL_DestroyTexture(rthrowtext[i]);
    }
}
