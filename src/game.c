#include "game.h"

void init_game(struct game *game, SDL_Renderer *render, SDL_Window *win,
        struct body *player, char *path)
{
        game->render = render;
        game->win = win;
        game->map.mob = NULL;
        game->map.objs = NULL;
        game->map.dblks = NULL;
        game->map.fblks = NULL;
        game->level = parsepath("./maps/path.txt");
        parsemap(path, &(game->map));
        game->player = *player;
        game->playing = 1;
        game->player.nbombs = 0;
        game->last_update_time = 0;
        game->light_render = 0;
        game->light_ptcs = NULL;
        game->bomb.enable = 0;
        game->bomb.light_ptcs = NULL;
        game->timer = 99;
}
