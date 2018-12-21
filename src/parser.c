#include "parser.h"

static int get_width(FILE *file)
{
    int width = 0;
    char c = 0;
    while((c = fgetc(file)) != '\n')
        width ++;
    if(fseek(file, 0, SEEK_SET) == 0)
        return width;
    else
    {
        fprintf(stderr, "Fail to set file postion : %s\n",
                strerror(errno));
        return -1;
    }
}

static int get_height(FILE *file)
{
    int height = 0;
    char c = fgetc(file);
    while(c != EOF)
    {
        if(c == '\n')
            height++;
        c = fgetc(file);
    }
    if(fseek(file, 0, SEEK_SET) == 0)
        return height;
    else
    {
        fprintf(stderr, "Fail to set file current postion : %s\n",
                strerror(errno));
        return -1;
    }
}

static int get_id(FILE *file)
{
    char *id = malloc(sizeof(char) * (ID_SIZE + 1));
    if(fseek(file,-ID_SIZE-1, SEEK_END) == 0)
    {
        char c = fgetc(file);
        int i = 0;
        while(c != EOF && c != '\n')
        {
            id[i] = c;
            i ++;
            c = fgetc(file);
        }
    }
    else
    {
        fprintf(stderr, "Fail to read id");
        return -1;
    }
    if(fseek(file, 0, SEEK_SET) == 0)
    {
        int n = atoi(id);
        free(id);
        return n;
    }
    else
    {
        fprintf(stderr, "Fail to set file current postion : %s\n",
                strerror(errno));
        return -1;
    }
}

static void match_obj(struct fifo *list_obj, struct obj *obj,
                      char c, int i, int j)
{
    obj = malloc(sizeof(struct obj));
    obj->pos.x = j * MAP_BLOCK_SIZE;
    obj->pos.y = i * MAP_BLOCK_SIZE;
    switch(c)
    {
        case 'B':
            obj->type = BOMBS;
        break;
    }
    fifo_push(list_obj, obj);
}

static void match_char(struct map *map, char c, char *line, int j, int i)
{
    if(map->mob == NULL)
        map->mob = fifo_init();
    if(map->objs == NULL)
        map->objs = fifo_init();
    if(map->dblks == NULL)
        map->dblks = fifo_init();
    if(map->fblks == NULL)
        map->fblks = fifo_init();
    struct mob *mob = NULL;
    struct obj *obj = NULL;
    struct dly_blk *dblk = NULL;
    struct vect2D *fblk = NULL;
    switch(c)
    {
        case '*':
            map->start.x = j;
            map->start.y = i;
            line[j] = '.';
        break;
        case '@':
            map->end.x = j;
            map->end.y = i;
            line[j] = '.';
        break;
        case '+':
            mob = malloc(sizeof(struct mob));
            mob->pos.x = j;
            mob->pos.y = i;
            fifo_push(map->mob, mob);
            line[j] = '.';
        break;
        case 'X':
            dblk = malloc(sizeof(struct dly_blk));
            dblk->pos.x = j * MAP_BLOCK_SIZE;
            dblk->pos.y = i * MAP_BLOCK_SIZE;
            fifo_push(map->dblks, dblk);
            line[j] = '.';
        break;
        case 'O':
            fblk = malloc(sizeof(struct vect2D));
            fblk->x = j * MAP_BLOCK_SIZE;
            fblk->y = i * MAP_BLOCK_SIZE;
            fifo_push(map->fblks, fblk);
            line[j] = 'O';
        break;
        default:
            if(c == 'B')
            {
                match_obj(map->objs, obj, c, i, j);
                line[j] = c;
            }
            else
                line[j] = c;
    }
}

static void fill_grid(struct map *map, FILE *file)
{
    char **g = malloc(sizeof(char *) * map->height);
    for(int i = 0; i < map->height; i++)
    {
        char *line = malloc(sizeof(char) * map->width);
        for(int j = 0; j < map->width; j++)
        {
            char c = fgetc(file);
            if(c != '\n')
                match_char(map, c, line, j, i);
            else
                j--;
        }
        g[i] = line;
    }
    map->grid = g;
}

void parsemap(char *filename, struct map *map)
{
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        fprintf(stderr, "Fail to open \"%s\" : %s\n",
                filename, strerror(errno));
        return;
    }
    map->width = get_width(file);
    map->height = get_height(file)-1;
    map->id = get_id(file);
    printf("%d\n",map->id);
    fill_grid(map,file);
    fclose(file);
    return;
}
