#include "physics.h"


double delta_time(uint64_t *last_update_time)
{
    double freq = SDL_GetPerformanceFrequency();

    uint64_t last = *last_update_time;
    uint64_t now = SDL_GetPerformanceCounter();

    *(last_update_time) = now;

    return (now - last) * 1000 / freq;
}

double dist(struct vect2D a, struct vect2D b)
{
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

static void update_player(struct body *player, struct map *map, double delta)
{
    player->vel.x += player->acc.x * delta;
    player->vel.y += player->acc.y * delta;

    float px = player->pos.x;
    float py = player->pos.y;

    player->pos.x += player->vel.x * delta;
    int x = (player->pos.x + ((player->vel.x) > 0
                ? HALF_SPRITE / 2 : -HALF_SPRITE / 2)) / MAP_BLOCK_SIZE;
    int y = player->pos.y / MAP_BLOCK_SIZE;
    if (y >= map->height)
        y = map->height - 1;
    if (x >= map->width)
        x = map->width - 1;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (map->grid[y][x] == '#' || map->grid[y][x] == 'O')
        player->pos.x = px;
    player->pos.y += player->vel.y * delta;
    x = player->pos.x / MAP_BLOCK_SIZE;
    y = (player->pos.y + ((player->vel.y) > 0
                ? HALF_SPRITE : -HALF_SPRITE)) / MAP_BLOCK_SIZE;
    if (y >= map->height)
        y = map->height - 1;
    if (x >= map->width)
        x = map->width - 1;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    player->onGround = 0;
    if (map->grid[y][x] == '#' || map->grid[y][x] == 'O')
    {
        if (y > py / MAP_BLOCK_SIZE && player->vel.y > 0)
            player->onGround = 1;
        player->pos.y = py;
    }

    player->acc.x = 0;
    player->acc.y = GRAVITY;
}

static void update_mob(struct mob *mob, struct map *map, double delta)
{
    mob->vel.x += mob->acc.x * delta;
    mob->vel.y += mob->acc.y * delta;

    float px = mob->pos.x;
    float py = mob->pos.y;

    mob->pos.x += mob->vel.x * delta;
    int x = (mob->pos.x + ((mob->vel.x) > 0
                ? HALF_SPRITE / 2 : -HALF_SPRITE / 2)) / MAP_BLOCK_SIZE;
    int y = mob->pos.y / MAP_BLOCK_SIZE;
    if (y >= map->height)
        y = map->height - 1;
    if (x >= map->width)
        x = map->width - 1;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (map->grid[y][x] == '#' || map->grid[y][x] == 'O')
        mob->pos.x = px;
    mob->pos.y += mob->vel.y * delta;
    x = mob->pos.x / MAP_BLOCK_SIZE;
    y = (mob->pos.y + ((mob->vel.y) > 0
                ? HALF_SPRITE : -HALF_SPRITE)) / MAP_BLOCK_SIZE;
    if (y >= map->height)
        y = map->height - 1;
    if (x >= map->width)
        x = map->width - 1;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    mob->onGround = 0;
    if (map->grid[y][x] == '#' || map->grid[y][x] == 'O')
    {
        if (y > py / MAP_BLOCK_SIZE && mob->vel.y > 0)
            mob->onGround = 1;
        mob->pos.y = py;
    }

    mob->acc.x = 0;
    mob->acc.y = GRAVITY;
}

static void update_mobs(struct game *game, double delta)
{
    struct list *cur = game->map.mob->head;
    struct mob *mob = NULL;
    for (; cur; cur = cur->next)
    {
        mob = cur->data;
        if (mob->track)
        {
            if (mob->pos.x < game->player.pos.x)
                mob->vel.x = MOB_SPEED;
            else
                mob->vel.x = -MOB_SPEED;
            update_mob(mob, &(game->map), delta);
            if (dist(mob->pos, game->player.pos) > MAX_TRACK_DIST)
                mob->track = 0;
        }
    }

}

static void update_light_ptc(struct map *map, struct light *ptc, double delta)
{
    float px = ptc->pos.x;
    float py = ptc->pos.y;

    ptc->pos.x += ptc->vel.x * delta;
    ptc->pos.y += ptc->vel.y * delta;
    int x = (ptc->pos.x + ((ptc->vel.x) > 0
                ? LIGHT_RADIUS : -LIGHT_RADIUS)) / MAP_BLOCK_SIZE;
    int y = ptc->pos.y / MAP_BLOCK_SIZE;
    if (y >= map->height)
        y = map->height - 1;
    if (x >= map->width)
        x = map->width - 1;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (map->grid[y][x] == '#')
    {
        ptc->pos.x = px;
        ptc->vel.x = -ptc->vel.x;

    }
    x = ptc->pos.x / MAP_BLOCK_SIZE;
    y = (ptc->pos.y + ((ptc->vel.y) > 0
                ? LIGHT_RADIUS : -LIGHT_RADIUS)) / MAP_BLOCK_SIZE;
    if (y >= map->height)
        y = map->height - 1;
    if (x >= map->width)
        x = map->width - 1;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (map->grid[y][x] == '#')
    {
        ptc->pos.y = py;
        ptc->vel.y = -ptc->vel.y;
    }

    ptc->brightness -= LIGHT_DECR;
}

static void update_light(struct game *game, double delta, struct light *ptcs,
        int n, int player_light)
{
    struct list *cur = NULL;
    struct mob *mob = NULL;
    struct light *ptc = NULL;
    for (int i = 0; i < n; i++)
    {
        cur = game->map.mob->head;
        ptc = &(ptcs[i]);
        update_light_ptc(&(game->map), ptc, delta);
        for (; cur; cur = cur->next)
        {
            mob = cur->data;
            if (dist(mob->pos, ptc->pos) < HALF_SPRITE)
                mob->track = player_light;
            if (!player_light)
            {
                struct list *fcur = game->map.fblks->head;
                struct vect2D *tmpos = NULL;
                for (; fcur; fcur = fcur->next)
                {
                    tmpos = fcur->data;
                    if (dist(*tmpos, ptc->pos) <= HALF_SPRITE)
                    {
                        int x = tmpos->x / MAP_BLOCK_SIZE;
                        int y = tmpos->y / MAP_BLOCK_SIZE;
                        game->map.grid[y][x] = ' ';
                    }
                }
            }
        }
    }
}

static void update_bomb(struct game *game, double delta)
{
    struct bomb *bomb = &(game->bomb);
    struct map *map = &(game->map);
    bomb->vel.x += bomb->acc.x * delta;
    bomb->vel.y += bomb->acc.y * delta;

    float px = bomb->pos.x;
    float py = bomb->pos.y;

    bomb->pos.x += bomb->vel.x * delta;
    int x = (bomb->pos.x + ((bomb->vel.x) > 0
                ? HALF_SPRITE / 2 : -HALF_SPRITE / 2)) / MAP_BLOCK_SIZE;
    int y = bomb->pos.y / MAP_BLOCK_SIZE;
    if (y >= map->height)
        y = map->height - 1;
    if (x >= map->width)
        x = map->width - 1;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (map->grid[y][x] == '#' || map->grid[y][x] == 'O')
        bomb->pos.x = px;
    bomb->pos.y += bomb->vel.y * delta;
    x = bomb->pos.x / MAP_BLOCK_SIZE;
    y = (bomb->pos.y + ((bomb->vel.y) > 0
                ? FULL_SPRITE : -FULL_SPRITE)) / MAP_BLOCK_SIZE;
    if (y >= map->height)
        y = map->height - 1;
    if (x >= map->width)
        x = map->width - 1;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (map->grid[y][x] == '#' || map->grid[y][x] == 'O')
    {
        if (y > py / MAP_BLOCK_SIZE && bomb->vel.y > 0)
            bomb->vel.x = 0;
        bomb->pos.y = py;
    }
    bomb->acc.x = 0;
    bomb->acc.y = GRAVITY;
    bomb->lifetime--;
}

void update_physics(struct game *game)
{
    if (!game->last_update_time)
    {
        delta_time(&(game->last_update_time));
        return;
    }
    double delta = delta_time(&(game->last_update_time));
    update_player(&(game->player), &(game->map), delta);
    update_mobs(game, delta);
    struct bomb *bomb = &(game->bomb);
    if (bomb->enable)
    {
        update_bomb(game, delta);
        if (bomb->lifetime <= 0)
        {
            if (!bomb->light_ptcs)
                bomb->light_ptcs = init_lights(bomb->pos, BOMB_PTC);
            update_light(game, delta, game->bomb.light_ptcs, BOMB_PTC, 0);
            if (bomb->light_ptcs[0].brightness <= 0)
            {
                bomb->enable = 0;
                free(bomb->light_ptcs);
            }
        }
    }
    if (game->light_render)
    {
        update_light(game, delta, game->light_ptcs, game->player.lights, 1);
        if (game->light_ptcs[0].brightness <= 0)
        {
            game->light_render = 0;
            game->player.lights = 0;
            free(game->light_ptcs);
            game->light_ptcs = NULL;
        }
    }
}
