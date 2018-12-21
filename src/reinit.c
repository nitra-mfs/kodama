#include "reinit.h"

void free_fifo(struct fifo *f)
{
    struct list *cur = f->head;
    for (; cur; cur = cur->next)
        free(cur->data);
    fifo_destroy(f);
}

void free_map(char **grid, int height)
{
    for (int i = 0; i < height; i++)
        free(grid[i]);
    free(grid);
}

void reinit_game(struct game *game, SDL_Renderer *render, SDL_Window *win,
       char *path)
{
        game->render = render;
        game->win = win;
        free_fifo(game->map.mob);
        free_fifo(game->map.objs);
        free_fifo(game->map.dblks);
        free_fifo(game->map.fblks);
        game->map.mob = NULL;
        game->map.objs = NULL;
        game->map.dblks = NULL;
        game->map.fblks = NULL;
        free_map(game->map.grid, game->map.height);
        game->map.grid = NULL;
        parsemap(path, &(game->map));
        game->bomb.enable = 0;
        game->bomb.light_ptcs = NULL;
        game->timer = 99;
}

void reinit_player(struct game *game)
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

    game->light_render = 0;
    game->player.lights = 0;
    if (!game->light_ptcs)
        free(game->light_ptcs);
    game->light_ptcs = NULL;
    game->player.time_light = 0;
    game->player.last_time_light = 0;

    player->end = 0;
    player->level = game->map.id;
    player->nbombs = 0;
}

static void reinit_mob(struct mob *mob)
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

void reinit_mobs(struct game *game)
{
    struct list *mob = game->map.mob->head;
    for (; mob; mob = mob->next)
        reinit_mob(mob->data);
}
