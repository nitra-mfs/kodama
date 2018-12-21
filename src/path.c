#include "path.h"

struct fifo *parsepath(char *filename)
{
    struct fifo *pathlist = fifo_init();
    FILE *file = fopen(filename, "r");
    char *buffer = malloc(sizeof(char) * BUF_SIZE);
    if(file == NULL)
    {
        fprintf(stderr, "Fail to open \"%s\" : %s\n",
                filename, strerror(errno));
        return NULL;
    }
    while(fgets(buffer, BUF_SIZE, file) != NULL)
    {
        char *path = malloc(sizeof(char) * BUF_SIZE);
        strcpy(path,buffer);
        path[strlen(buffer)-1] = '\0';
        fifo_push(pathlist, path);
    }
    free(buffer);
    fclose(file);
    return pathlist;
}
