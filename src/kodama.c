#include "kodama.h"

int lastdir;

char *get_path(struct game *game)
{
    int il = game->player.level - 1;
    int i = 0;
    struct list *path = game->level->head;
    char *str = path->data;
    while (path && i != il)
    {
        i++;
        path = path->next;
    }

    str = path->data;
    return str;
}

int reinit(struct game *game, struct hud *hud)
{
    if (game->player.level >  NB_MAP)
    {
        title_run(game, hud);
        game->playing = 0;
        return 0;
    }

    char *path = get_path(game);
    if (game->player.end)
    {
        reinit_game(game, game->render, game->win, path);
        reinit_player(game);
        reinit_mobs(game);
        return 1;
    }
    reinit_game(game, game->render, game->win, path);
    reinit_player(game);
    reinit_mobs(game);
    return 0;
}

int run(struct game *game, struct hud *hud)
{
    int reload = 0;
    char nbomb[64] = "";
    struct text gtext;
    struct text gptext;
    uint64_t last = 0;
    delta_time(&last);
    while (game->playing)
    {
        if (game->player.level > 9)
            game->player.level = game->map.id;

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                game->playing = 0;
                break;
            }
            else if (e.type == SDL_KEYDOWN)
                action(game, &e);
            else if (e.type == SDL_KEYUP)
            {
                int sym = e.key.keysym.sym;
                if (sym != SDLK_UP)
                {
                    if (sym == SDLK_LEFT || sym == SDLK_RIGHT || sym == SDLK_a
                            || sym == SDLK_d)
                        game->player.vel.x = 0;
                    else if (sym == SDLK_SPACE && !game->light_render)
                    {
                        game->player.lights *= 5;
                        game->player.lights += 20;
                        game->player.time_light = 0;
                        game->player.last_time_light = 0;
                        game->light_render = 1;
                        game->light_ptcs = init_lights(game->player.pos, 
                                game->player.lights);
                    }
                    else if (sym == SDLK_b && !game->bomb.enable 
                            && game->player.nbombs)
                    {
                        game->player.astate = 3;
                        game->player.throwidx = 0;
                        game->player.throwlat = 100;
                        game->bomb.pos.x = game->player.pos.x;
                        game->bomb.pos.y = game->player.pos.y - MAP_BLOCK_SIZE;
                        game->bomb.lifetime = BOMB_LIFETIME;
                        game->bomb.acc.x = game->player.vel.x;
                        game->bomb.vel.y = JUMP_VEL;
                        game->bomb.light_ptcs = NULL;
                        game->bomb.enable = 1;
                        game->player.nbombs--;
                    }
                    else if (sym == SDLK_p)
                        game->player.nbombs = 99;
                    else if (sym == SDLK_x)
                        game->player.end = 1;
                    if (game->player.astate == 1 || game->player.astate == 2)
                        game->player.astate = 0;
                }
            }
        }

        int timer = game->timer;
        sprintf(nbomb, "Bombs: %d    Timer: %d", game->player.nbombs, timer);
        init_text(&gptext, "", 100, 2, 1);
        init_text(&gtext, nbomb, 30, 0.5, 0.1);
        init_hud(game, hud, &gtext, &gptext);
        SDL_RenderClear(game->render);
        update_physics(game);
        render_frame(game);
        update_hudp(game, hud);
        SDL_RenderPresent(game->render);
        clear_hud(hud);

        game->timer -= delta_time(&last) / 1000;

        if (game->timer <= 0)
            game->player.isAlive = 0;

        if (game->player.isAlive == 0)
        {
            Mix_Resume(-1);
            title_run(game, hud);
            Mix_Pause(-1);
            reload = reinit(game, hud);
        }

        if (game->player.end)
        {
            struct text text;
            struct text ptext;
            init_text(&ptext, "", 100, 2, 1);
            init_text(&text, "You complete this level !", 100, 2, 1);
            init_hud(game, hud, &text, &ptext);
            SDL_RenderClear(game->render);
            update_hudp(game, hud);
            SDL_RenderPresent(game->render);
            SDL_Delay(1000);
            clear_hud(hud);
            game->player.level++;
            reload = reinit(game, hud);
        }
    }

    return reload;
}


int main(void)
{
    lastdir = 0;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDl could not init\n");
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Kodama", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!win)
    {
        printf("Win could not be created\n");
        return 0;
    }

    SDL_Renderer *render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);

    struct body player;
    struct game game;
    struct hud hud;
    hud.texture = NULL;
    hud.ptexture = NULL;

    Mix_Chunk *sound = NULL;
    Mix_Music *music = NULL;
    init_game(&game, render, win, &player, "./maps/m1.kmf");
    init_player(&game, render);
    init_mobs(&game);
    init_audio();
    parserun(&game);
    parserrun(&game);
    parsethrow(&game);
    rparsethrow(&game);

    if (TTF_Init() == -1)
    {
        printf("TTF could not start");
        return 1;
    }

    SDL_Surface *surf = IMG_Load("./res/skm64x64.png");
    game.mobtext = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);
    surf = IMG_Load("./res/bomb32x32.png");
    game.bombtext = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);

    playmusic(music, "./res/song.ogg");
    playeffect(sound, "./res/wind.wav");
    title_run(&game, &hud);
    Mix_Pause(-1);

    while (run(&game, &hud))
        continue;

    anim_destroy();
    free_fifo(game.level);
    free_fifo(game.map.mob);
    free_fifo(game.map.objs);
    free_fifo(game.map.fblks);
    free_fifo(game.map.dblks);
    free_map(game.map.grid, game.map.height);
    SDL_DestroyTexture(game.mobtext);
    SDL_DestroyTexture(game.bombtext);
    SDL_DestroyTexture(game.player.text);
    Mix_FreeChunk(sound);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
    destroy_fonts();
    TTF_Quit();
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
