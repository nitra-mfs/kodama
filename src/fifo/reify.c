#include "fifo.h"
#include <stdlib.h>

struct fifo *fifo_init(void)
{
    struct fifo *f = malloc(sizeof(struct fifo));
    if (!f)
        return NULL;
    f->head = NULL;
    f->tail = NULL;
    f->size = 0;
    return f;
}

void fifo_clear(struct fifo *fifo)
{
    if (!fifo)
        return;
    struct list *curr = fifo->head;
    struct list *tmp = NULL;
    while (curr)
    {
        tmp = curr->next;
        free(curr);
        curr = tmp;
    }
    fifo->size = 0;
}

void fifo_destroy(struct fifo *fifo)
{
    fifo_clear(fifo);
    free(fifo);
}
