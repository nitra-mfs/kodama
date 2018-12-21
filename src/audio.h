#pragma once

#include "define.h"

int init_audio(void);
int playmusic(Mix_Music *music, char *path);
int playeffect(Mix_Chunk *sound, char *path);
