#include "fifo.h"
#include <stdlib.h>

void fifo_push(struct fifo *fifo, void *elt)
{
    struct list *el = malloc(sizeof(struct list));
    if (!el)
        return;
    el->data = elt;
    el->next = NULL;
    if (!fifo->size)
        fifo->head = el;
    else
        fifo->tail->next = el;
    fifo->tail = el;
    fifo->size++;
}

void fifo_pop(struct fifo *fifo)
{
    if (!fifo->size)
        return;
    struct list *tmp = fifo->head;
    fifo->head = tmp->next;
    free(tmp);
    fifo->size--;
    if (!fifo->size)
        fifo->tail = NULL;
}

void fifo_push_front(struct fifo *fifo, void *elt)
{
    struct list *el = malloc(sizeof(struct list));
    if (!el)
        return;
    el->data = elt;
    el->next = fifo->head;
    if (!fifo->size)
        fifo->tail = el;
    fifo->head = el;
    fifo->size++;
}
