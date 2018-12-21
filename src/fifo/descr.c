#include "fifo.h"

void *fifo_head(struct fifo *fifo)
{
    if (!fifo || !fifo->size)
        return NULL;
    return fifo->head->data;
}
