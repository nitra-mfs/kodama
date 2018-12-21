#include "checker.h"

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

static void printmsg(char *msg, int b)
{
    if(b == 1)
        printf("["GREEN("SUCCES")"] : %s\n", msg);
    else
        printf("["RED("FAIL")"] : %s\n", msg);
}

static int unkchar(FILE *file)
{
    char c = fgetc(file);
    while(c != EOF)
    {
        if(c != ' ' && c != '#' &&
           c != '*' && c != '@' &&
           c != 'B' && c != '+' &&
           c != 'O' && c != 'X' &&
           c != '\n' && !(c > 48 && c <= 57))
        {
           printmsg("unknow char",0);
           fseek(file, 0, SEEK_SET);
           return 0;
        }
        c = fgetc(file);
    }
    printmsg("unknow char",1);
    fseek(file, 0, SEEK_SET);
    return 1;
}

static int rectangle(FILE *file)
{
    char *buffer = malloc(sizeof(char) * BUF_SIZE);
    fgets(buffer, BUF_SIZE, file);
    size_t ref = strlen(buffer);
    while(fgets(buffer, BUF_SIZE, file) != NULL)
    {
        if(strlen(buffer) != ref && strlen(buffer) != 2)
        {
            printmsg("map is rectangle",0);
            free(buffer);
            fseek(file, 0, SEEK_SET);
            return 0;
        }
    }
    printmsg("map is rectangle",1);
    free(buffer);
    fseek(file, 0, SEEK_SET);
    return 1;
}

static int startend(FILE *file)
{
    int s = 0;
    int e = 0;
    char c = fgetc(file);
    while(c != EOF)
    {
        if(c == '*')
            s ++;
        if(c == '@')
            e ++;
        c = fgetc(file);
    }
    if(e == 1 && s == 1)
    {
        printmsg("one start, one end",1);
        fseek(file, 0, SEEK_SET);
        return 1;
    }
    else
    {
        printmsg("one start, one end",0);
        fseek(file, 0, SEEK_SET);
        return 0;
    }
}

static int border(FILE *file)
{
    int h = get_height(file);
    int w = get_width(file);
    char *buffer = malloc(sizeof(char) * BUF_SIZE);
    fgets(buffer, BUF_SIZE, file);
    char *fl = malloc(sizeof(char) * strlen(buffer));
    strcpy(fl,buffer);
    for(int i = 2; i < h; i++)
    {
        fgets(buffer, BUF_SIZE, file);
        if(buffer[0] != '#' || buffer[w-1] != '#')
        {
            printmsg("correct border",0);
            fseek(file, 0, SEEK_SET);
            return 0;
        }
    }
    for(int i = 0; i < w; i++)
    {
        if(buffer[i] != fl[i] || fl[i] != '#' || buffer[i] != '#')
        {
            printmsg("correct border",0);
            fseek(file, 0, SEEK_SET);
            return 0;
        }
    }
    printmsg("correct border",1);
    fseek(file, 0, SEEK_SET);
    return 1;
}

void checker(char *filename)
{
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        fprintf(stderr, "Fail to open \"%s\" : %s\n",
                filename, strerror(errno));
        return;
    }
    int u = unkchar(file);
    int r = rectangle(file);
    int s = startend(file);
    int b = border(file);
    if(u == 1 && r == 1 && s == 1 && b == 1)
        printf(GREEN("===VALID MAP===\n"));
    else
        printf(RED("===UNVALID MAP===\n"));
    fclose(file);
}

void checkall(char * path)
{
    DIR * d = opendir(path);
    char *buffer = malloc(sizeof(char) * BUF_SIZE);
    if(d==NULL)
        return;
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL)
    {
        if(dir->d_type != DT_DIR && strcmp(dir->d_name,"path.txt") != 0)
        {
            sprintf(buffer,"%s%s%s","maps/",dir->d_name,"\0");
            printf("%s\n",dir->d_name);
            checker(buffer);
        }
    }
    closedir(d);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr,"Give a map name or call the rule all\n");
        return 0;
    }
    if(strcmp(argv[1],"all") == 0)
        checkall("./maps");
    else
        checker(argv[1]);
    return 1;
}
