#pragma once

#include "define.h"
#include "game.h"
#include "body.h"
#include "light.h"

double delta_time(uint64_t *last_update_time);
double dist(struct vect2D a, struct vect2D b);
void update_physics(struct game *game);
