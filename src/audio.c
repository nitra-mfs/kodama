#include "audio.h"

int playmusic(Mix_Music *music, char *path)
{
    music = Mix_LoadMUS(path);
    if (!music)
        return 1;
    if (Mix_PlayMusic(music, -1) == -1)
        return 1;
    return 0;
}

int playeffect(Mix_Chunk *sound, char *path)
{
    sound = Mix_LoadWAV(path);
    if (!sound)
        return 1;
    if (Mix_PlayChannel(-1, sound, -1) == -1)
        return 1;
    return 0;
}

int init_audio(void)
{
    if (Mix_Init(MIX_INIT_OGG|MIX_INIT_MP3) == -1)
        return 1;
    if (Mix_OpenAudio( MIX_DEFAULT_FREQUENCY
                , MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        return 1;

    return 0;
}
