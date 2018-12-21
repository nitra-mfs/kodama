#pragma once

#define WIDTH 1200
#define HEIGHT 800

#define GRAVITY 0.0005
#define JUMP_VEL -0.4
#define SPEED 0.2
#define MOB_SPEED 0.1

#define MAP_BLOCK_SIZE 64
#define FULL_SPRITE 64
#define HALF_SPRITE 32

#define BLINK_TIME 500

#define LIGHT_CHARGE_TIME 0.01
#define LIGHT_RADIUS 2
#define LIGHT_DECR 0.075

#define BOMB_LIFETIME 3000
#define BOMB_PTC 50

#define M_PI 3.14159265358979

#define NB_MAP 9

#define MAX_TRACK_DIST (MAP_BLOCK_SIZE * 5)

#define RUN_SIZE 17

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>
