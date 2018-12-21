#ifndef _PARSER_H
#define _PARSER_H

#define ID_SIZE 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "game.h"
#include "mobs.h"
#include "fifo/fifo.h"
#include "objs.h"
#include "dly_blk.h"


struct map;

void parsemap(char *filename, struct map *map);

#endif
