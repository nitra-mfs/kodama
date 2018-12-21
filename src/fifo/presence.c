#include "fifo.h"
#include "node.h"

int inlist(struct fifo *f, int x, int y)
{
    struct list *cur = f->head;
    struct node *n = NULL;
    while (cur != NULL)
    {
        n = cur->data;
        if (n->x == x && n->y == y)
            return 1;
        cur = cur->next;
    }

    return 0;
}
