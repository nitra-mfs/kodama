#include "body.h"

void init_player(struct game *game, SDL_Renderer *render)
{
    struct body *player = &(game->player);

    player->pos.x = game->map.start.x * MAP_BLOCK_SIZE + HALF_SPRITE;
    player->pos.y = game->map.start.y * MAP_BLOCK_SIZE + HALF_SPRITE;

    player->vel.x = 0;
    player->vel.y = 0;

    player->acc.x = 0;
    player->acc.y = 0;

    player->isAlive = 1;
    player->onGround = 0;

    player->lights = 0;
    player->last_time_light = 0;
    player->time_light = 0;

    player->end = 0;
    player->level = game->map.id;

    player->runidx = 0;
    player->rrunidx = 0;
    player->astate = 0;

    SDL_Surface *surf = IMG_Load("./res/sk64x64.png");
    player->text = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);
}
