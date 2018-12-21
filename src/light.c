#include "light.h"

struct light *init_lights(struct vect2D pos, int n)
{
    struct light *ptcs = malloc(n * sizeof(struct light));
    double ang = 0;
    for (int i = 0; i < n; i++)
    {
        ptcs[i].pos.x = pos.x;
        ptcs[i].pos.y = pos.y;
        ptcs[i].brightness = 255;
        ptcs[i].vel.x = cos(ang) / 2;
        ptcs[i].vel.y = sin(ang) / 2;
        ang += 2 * M_PI / n;
    }
    return ptcs;
}
