#include "mobs.h"

void init_mob(struct mob *mob)
{
    mob->pos.x *= MAP_BLOCK_SIZE;
    mob->pos.y *= MAP_BLOCK_SIZE;
    mob->pos.x += HALF_SPRITE;
    mob->pos.y += HALF_SPRITE;

    mob->vel.x = 0;
    mob->vel.y = 0;

    mob->acc.x = 0;
    mob->acc.y = 0;

    mob->track = 0;
    mob->onGround = 0;
}

void init_mobs(struct game *game)
{
    struct list *mob = game->map.mob->head;
    for (; mob; mob = mob->next)
        init_mob(mob->data);
}

/*void init_npc(struct mob *npc, SDL_Renderer *render)
{
    SDL_Surface *surf = SDL_LoadBMP("./sknpc64x64.bmp");
    npc->text = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);
}

void init_npcs(struct game *game)
{
    struct list *npc = game->map.npc->head;
    for (; npc; npc = npc->next)
        init_npc(npc->data, game->render);
}*/
