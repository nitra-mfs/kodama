#pragma once

#include "define.h"
#include "game.h"

void free_fifo(struct fifo *f);
void free_map(char **grid, int height);
void reinit_game(struct game *game, SDL_Renderer *render, SDL_Window *win,
        char *path);
void reinit_player(struct game *game);
void reinit_mobs(struct game *game);
