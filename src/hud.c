#include "hud.h"
#include "game.h"

static TTF_Font *fonts[5] = { NULL };

static int sizes[] =
{
    30, 64, 75, 100, 256
};

int update_hud(struct game *game, struct hud *hud)
{
    SDL_RenderCopy(game->render, hud->texture, NULL, &(hud->dst_rect));
    SDL_RenderCopy(game->render, hud->ptexture, NULL, &(hud->pdst_rect));
    return 0;
}

int update_hudp(struct game *game, struct hud *hud)
{
    SDL_RenderCopy(game->render, hud->texture, NULL, &(hud->dst_rect));
    return 0;
}

void clear_hud(struct hud *hud)
{
    SDL_DestroyTexture(hud->texture);
    SDL_DestroyTexture(hud->ptexture);
}

int init_hud(struct game *game, struct hud *hud, struct text *text, 
        struct text *ptext)
{
    int texW;
    int texH;

    int ptexW;
    int ptexH;

    hud->running = 1;

    SDL_Color white = { 255, 255, 255, 255 };

    if (!fonts[0])
    {
        fonts[0] = TTF_OpenFont("./res/Lobster-Regular.ttf", 30);
        fonts[1] = TTF_OpenFont("./res/Lobster-Regular.ttf", 64);
        fonts[2] = TTF_OpenFont("./res/Lobster-Regular.ttf", 75);
        fonts[3] = TTF_OpenFont("./res/Lobster-Regular.ttf", 100);
        fonts[4] = TTF_OpenFont("./res/Lobster-Regular.ttf", 256);
    }
    TTF_Font *font = NULL;
    TTF_Font *pfont = NULL;
    for (int i = 0; i < 5; i++)
        if (sizes[i] == text->size)
            font = fonts[i];
    for (int i = 0; i < 5; i++)
        if (sizes[i] == ptext->size)
            pfont = fonts[i];

    SDL_Surface *surface = TTF_RenderText_Solid(font, text->text, white);
    SDL_Surface *psurface = TTF_RenderText_Solid(pfont, ptext->text, white);

    hud->texture = SDL_CreateTextureFromSurface(game->render, surface);
    hud->ptexture = SDL_CreateTextureFromSurface(game->render, psurface);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(psurface);

    SDL_QueryTexture(hud->texture, NULL, NULL, &texW, &texH);
    SDL_QueryTexture(hud->ptexture, NULL, NULL, &ptexW, &ptexH);

    SDL_Rect *dst_rect = &(hud->dst_rect);
    dst_rect->x = WIDTH / 2 - texW / text->x;
    dst_rect->y = HEIGHT / 2 - texH / text->y;
    dst_rect->w = texW;
    dst_rect->h = texH;

    SDL_Rect *pdst_rect = &(hud->pdst_rect);
    pdst_rect->x = WIDTH / 2 - ptexW / ptext->x;
    pdst_rect->y = HEIGHT / 1.5 - ptexH / ptext->y;
    pdst_rect->w = ptexW;
    pdst_rect->h = ptexH;

    return 0;
}

int init_text(struct text *text, char *msg, int size, double x, double y)
{
    text->text = msg;
    text->size = size;
    text->x = x;
    text->y = y;
    return 0;
}

int title_run(struct game *game, struct hud *hud)
{
    struct text text;
    struct text ptext;

    if (game->player.level > NB_MAP)
    {
        init_text(&text, "Congratulation you complete the game !", 75, 2, 1);
        init_text(&ptext, "press enter to quit", 64, 2, 2);
    }
    else if (game->player.isAlive)
    {
        init_text(&text, "Kodama", 256, 2, 1.5);
        init_text(&ptext, "press enter to start", 64, 2, 2);
    }
    else
    {
        init_text(&text, "Game over", 256, 2, 1.5);
        init_text(&ptext, "press enter to restart", 64, 2, 2);
    }
    init_hud(game, hud, &text, &ptext);
    uint64_t last_up = 0;
    delta_time(&last_up);
    double tps = 0;
    int p = 0;
    SDL_Event e;
    while (SDL_PollEvent(&e))
        continue;
    while (hud->running)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                hud->running = 0;
                break;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_RETURN)
                    hud->running = 0;
            }
        }
        tps += delta_time(&last_up);
        if (tps > BLINK_TIME)
        {
            tps = 0;
            p = !p;
        }
        SDL_RenderClear(game->render);
        if (p)
            update_hudp(game, hud);
        else
            update_hud(game, hud);
        SDL_RenderPresent(game->render);
    }
    clear_hud(hud);
    return 0;
}

void destroy_fonts(void)
{
    for (int i = 0; i < 5; i++)
        TTF_CloseFont(fonts[i]);
}
