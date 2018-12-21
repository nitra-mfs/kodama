#pragma once

#include "define.h"
#include "game.h"
#include "physics.h"

struct text
{
    char *text;
    int size;
    double x;
    double y;
};

struct hud
{
    SDL_Texture *texture;
    SDL_Texture *ptexture;
    SDL_Rect dst_rect;
    SDL_Rect pdst_rect;
    int running;
};


int update_hud(struct game *game, struct hud *hud);
int update_hudp(struct game *game, struct hud *hud);
void clear_hud(struct hud *hud);
int init_hud(struct game *game, struct hud *hud,
        struct text *text, struct text *ptext);
int init_text(struct text *text, char *msg, int size, double x, double y);
int title_run(struct game *game, struct hud *hud);
void destroy_fonts(void);
