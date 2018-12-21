#pragma once

#define BUF_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "fifo/fifo.h"

struct fifo *parsepath(char *filename);
