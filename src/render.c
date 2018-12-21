#include "render.h"

extern int lastdir;

static void render_map(struct map *map, SDL_Renderer *render,
        struct vect2D *pos)
{
    struct SDL_Rect rect;
    rect.w = MAP_BLOCK_SIZE;
    rect.h = MAP_BLOCK_SIZE;
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);

    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            if (i == map->end.y && j == map->end.x)
                SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
            else
                SDL_SetRenderDrawColor(render, 0, 0, 0, 255);

            rect.x = j * MAP_BLOCK_SIZE - pos->x + WIDTH / 2;
            rect.y = i * MAP_BLOCK_SIZE - pos->y + HEIGHT / 2;
            if (rect.x > WIDTH * MAP_BLOCK_SIZE || rect.x + MAP_BLOCK_SIZE < 0)
                continue;
            if (rect.y > HEIGHT * MAP_BLOCK_SIZE || rect.y + MAP_BLOCK_SIZE < 0)
                continue;
            SDL_RenderFillRect(render, &rect);
        }
    }
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
}

static void render_player(struct game *game, SDL_Renderer *render)
{
    struct body *player = &(game->player);
    struct SDL_Rect *rect = &(player->winpos);
    rect->x = WIDTH / 2 - HALF_SPRITE;
    rect->y = HEIGHT / 2 - HALF_SPRITE;
    rect->w = FULL_SPRITE;
    rect->h = FULL_SPRITE;
    if (player->astate == 1)
        SDL_RenderCopy(render, player->runtext[player->runidx], NULL, rect);
    else if (player->astate == 2)
        SDL_RenderCopy(render, player->rruntext[player->rrunidx], NULL, rect);
    else if (player->astate == 0)
        SDL_RenderCopy(render, player->text, NULL, rect);
    else if (player->astate == 3)
    {
        if (!lastdir)
            SDL_RenderCopy(render, player->throwtext[player->throwidx], NULL, rect);
        else
            SDL_RenderCopy(render, player->rthrowtext[player->throwidx], NULL, rect);

        player->throwlat--;
        if (player->throwlat <= 0)
        {
            player->throwlat = 100;
            player->throwidx++;
        }
        if (player->throwidx >= 28)
            player->astate = 0;
    }


    struct vect2D tmp;
    tmp.x = game->player.pos.x / MAP_BLOCK_SIZE;
    tmp.y = game->player.pos.y / MAP_BLOCK_SIZE;
    double dt = FULL_SPRITE;
    if (dist(game->map.end, tmp) <= dt / MAP_BLOCK_SIZE)
        game->player.end = 1;
}

/*
static void render_npc(struct npc *npc, SDL_Renderer *render)
{
    int wd = 0;
    int ht = 0;
    SDL_QueryTexture(npc->text, NULL, NULL, &wd, &ht);
    //struct SDL_Rect *rect = &(player->winpos);
    //rect->x = WIDTH / 2 - wd / 2;
    //rect->y = HEIGHT / 2 - ht / 2;
    //rect->w = wd;
    //rect->h = ht;
    SDL_RenderCopy(render, npc->text, NULL, rect);

}*/

/*
static void render_npcs(struct game *game)
{
    struct list *npc = game->map->npc->head;

    while (npc)
    {
        init_npc(npc->data, game->render);
        npc = npc->next;
    }
}
*/

static void render_mob(struct mob *mob, SDL_Renderer *render,
        struct vect2D *pos, SDL_Texture *text)
{
    struct SDL_Rect rect;
    rect.x = WIDTH / 2 - HALF_SPRITE - pos->x + mob->pos.x;
    rect.y = HEIGHT / 2 - HALF_SPRITE - pos->y + mob->pos.y;
    rect.w = FULL_SPRITE;
    rect.h = FULL_SPRITE;
    SDL_RenderCopy(render, text, NULL, &rect);
}

static void render_mobs(struct game *game)
{
    struct list *mob = game->map.mob->head;
    for (; mob; mob = mob->next)
    {
        render_mob(mob->data, game->render, &(game->player.pos), game->mobtext);
        struct mob *tmp = mob->data;
        if (dist(game->player.pos, tmp->pos) <= HALF_SPRITE)
            game->player.isAlive = 0;
    }
}

static void render_obj(struct obj *obj, struct game *game)
{
    struct SDL_Rect rect;
    rect.x = WIDTH / 2 + HALF_SPRITE / 2 - game->player.pos.x + obj->pos.x;
    rect.y = HEIGHT / 2 + HALF_SPRITE - game->player.pos.y + obj->pos.y;
    rect.w = HALF_SPRITE;
    rect.h = HALF_SPRITE;
    SDL_RenderCopy(game->render, game->bombtext, NULL, &rect);
}

static void render_objs(struct game *game)
{
    struct list *cur = game->map.objs->head;
    struct list *prec = NULL;
    for (; cur; prec = cur, cur = cur->next)
    {
        render_obj(cur->data, game);
        struct obj *tmp = cur->data;
        if (dist(game->player.pos, tmp->pos) <= FULL_SPRITE)
        {
            game->player.nbombs++;
            if (prec == NULL)
                game->map.objs->head = cur->next;
            else
                prec->next = cur->next;
            free(cur->data);
            free(cur);
            prec = NULL;
            cur = game->map.objs->head;
            if (!cur)
                break;
            continue;
        }
    }
}

static void render_lights(struct game *game, struct light *ptcs, int n)
{
    struct SDL_Rect rect;
    rect.w = LIGHT_RADIUS * 2;
    rect.h = LIGHT_RADIUS * 2;
    int brt = 0;
    for (int i = 0; i < n; i++)
    {
        brt = ptcs[i].brightness;
        SDL_SetRenderDrawColor(game->render, brt, brt, brt, 255);
        rect.x = WIDTH / 2 - LIGHT_RADIUS - game->player.pos.x + ptcs[i].pos.x;
        rect.y = HEIGHT / 2 - LIGHT_RADIUS - game->player.pos.y + ptcs[i].pos.y;
        SDL_RenderFillRect(game->render, &rect);
    }
    SDL_SetRenderDrawColor(game->render, 0, 0, 0, 255);
}

static void check_dblock(struct game *game)
{
    struct list *dblock = game->map.dblks->head;
    for(; dblock; dblock = dblock->next)
    {
        struct dly_blk *tmp = dblock->data;
        if (dist(game->player.pos, tmp->pos) <= HALF_SPRITE)
            game->player.isAlive = 0;
    }
}

static void render_bomb(struct bomb *bomb, struct game *game)
{
    struct SDL_Rect rect;
    rect.x = WIDTH / 2 + HALF_SPRITE / 2 - game->player.pos.x + bomb->pos.x;
    rect.y = HEIGHT / 2 + HALF_SPRITE - game->player.pos.y + bomb->pos.y;
    rect.w = HALF_SPRITE;
    rect.h = HALF_SPRITE;
    SDL_RenderCopy(game->render, game->bombtext, NULL, &rect);
}


void render_frame(struct game *game)
{
    render_map(&(game->map), game->render, &(game->player.pos));
    render_player(game, game->render);
    render_mobs(game);
    check_dblock(game);
    render_objs(game);
    if (game->bomb.enable && !game->bomb.light_ptcs)
        render_bomb(&(game->bomb), game);
    if (game->bomb.light_ptcs && game->bomb.enable)
        render_lights(game, game->bomb.light_ptcs, BOMB_PTC);
    if (game->light_render)
        render_lights(game, game->light_ptcs, game->player.lights);
    //render_npc(&game);
}
