#include "control.h"

extern int lastdir;

int action(struct game *game, SDL_Event *e)
{
    int k = e->key.keysym.sym;

    struct body *player = &(game->player);

    if (k == SDLK_ESCAPE)
        game->playing = 0;
    else if (k == SDLK_RIGHT || k == SDLK_d)
    {
        lastdir = 0;
        if (player->astate == 0)
            player->astate = 1;
        player->runidx++;
        if (player->runidx == RUN_SIZE)
            player->runidx = 0;
        player->vel.x = SPEED;
    }
    else if (k == SDLK_LEFT || k == SDLK_a)
    {
        lastdir = 1;
        if (player->astate == 0)
            player->astate = 2;
        player->rrunidx++;
        if (player->rrunidx == RUN_SIZE)
            player->rrunidx = 0;
        player->vel.x = -SPEED;
    }
    else if (k == SDLK_UP || k == SDLK_w)
    {
        if (player->onGround)
            player->vel.y = JUMP_VEL;
    }
    else if (k == SDLK_SPACE && !game->light_render)
    {
        if (!player->last_time_light)
            delta_time(&(player->last_time_light));
        player->time_light += delta_time(&(player->last_time_light));
        if (player->time_light > LIGHT_CHARGE_TIME)
        {
            player->time_light = 0;
            player->lights++;
        }
    }

    return 0;
}
