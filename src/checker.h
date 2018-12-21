#pragma once

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#define RED(string) "\x1b[31m" string "\x1b[0m"
#define GREEN(string) "\x1b[32m" string "\x1b[0m"

#define BUF_SIZE 256

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
