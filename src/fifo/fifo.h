#ifndef FIFO_H
# define FIFO_H

# include <stddef.h>

struct list
{
    void *data;
    struct list *next;
};

struct fifo
{
    struct list *head;
    struct list *tail;
    size_t size;
};

struct fifo *fifo_init(void);
void fifo_push(struct fifo *fifo, void *elt);
void fifo_push_front(struct fifo *fifo, void *elt);
void *fifo_head(struct fifo *fifo);
void fifo_pop(struct fifo *fifo);
void fifo_clear(struct fifo *fifo);
void fifo_destroy(struct fifo *fifo);
int inlist(struct fifo *fifo, int x, int y);

#endif /* !FIFO_H */
