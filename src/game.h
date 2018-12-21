#pragma once

#include "define.h"
#include "parser.h"
#include "vect2D.h"
#include "fifo/fifo.h"
#include "path.h"

struct map
{
    int width;
    int height;
    int id;
    char **grid;
    struct vect2D start;
    struct vect2D end;
    struct fifo *mob;
    struct fifo *objs;
    struct fifo *dblks;
    struct fifo *fblks;
};

struct body
{
    struct vect2D pos;
    struct vect2D vel;
    struct vect2D acc;
    int isAlive;
    int onGround;
    int end;
    int level;
    int nbombs;

    int astate;
    SDL_Texture **runtext;
    int runidx;
    SDL_Texture **rruntext;
    int rrunidx;
    SDL_Texture **throwtext;
    SDL_Texture **rthrowtext;
    int throwidx;
    int throwlat;

    int lights;
    uint64_t last_time_light;
    double time_light;

    SDL_Texture *text;
    SDL_Rect winpos;
};

struct mob
{
    struct vect2D pos;
    struct vect2D vel;
    struct vect2D acc;
    int track;
    int onGround;
};

struct npc
{
    struct vect2D pos;
    SDL_Texture *text;
};

struct light
{
    struct vect2D vel;
    struct vect2D pos;
    double brightness;
};

enum obj_type
{
    BOMBS
};

struct obj
{
    struct vect2D pos;
    enum obj_type type;
};

struct bomb
{
    struct vect2D pos;
    struct vect2D vel;
    struct vect2D acc;
    int lifetime;
    int enable;
    struct light *light_ptcs;
};

struct game
{
    SDL_Renderer *render;
    SDL_Window *win;

    struct fifo *level;

    struct map map;
    struct body player;
    int playing;
    uint64_t last_update_time;
    int light_render;
    struct light *light_ptcs;
    struct bomb bomb;
    double timer;
    SDL_Texture *mobtext;
    SDL_Texture *bombtext;
};

void init_game(struct game *game, SDL_Renderer *render, SDL_Window *win,
        struct body *player, char *path);

void init_map_texture(struct game *map);
