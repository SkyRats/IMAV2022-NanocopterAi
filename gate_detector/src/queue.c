#include <stdlib.h>
#include "queue.h"

Queue * createQueue(uint32_t size)
{
    Queue *q = NULL;
    q = malloc(sizeof(Queue));
    q->queueItem = malloc(size * sizeof(uint16_t));
    q->max = size;
    q->head = 0;
    q->tail = 0;

    return q;
}

void destroyQueue(Queue * f)
{
    free(f->queueItem);
    free(f);
}

bool queueIsEmpty(Queue * f)
{
    return (f->head == f->tail);
}

void increaseQueue(Queue * f)
{
    uint16_t *v2 = malloc(2 * f->max * sizeof(uint16_t));
    int i, k;

    for(i = f->head, k = 0; i != f->tail; i = (i + 1)%f->max)
        v2[k] = f->queueItem[i];

    free(f->queueItem);

    f->head = 0;
    f->tail = f->max - 1;
    f->max *= 2;
    f->queueItem = v2;
}

uint16_t queuePeek(Queue * f)
{
    if(!queueIsEmpty(f))
        return f->queueItem[f->head];
    return (uint16_t)65535; /* 2^16 -1*/
}

void enqueue(Queue * f, uint16_t X)
{
    if(f->head == (f->tail + 1)%f->max) /* Queue is full */
        increaseQueue(f);

    f->queueItem[f->tail] = X;
    f->tail = (f->tail + 1)%f->max;
}

uint16_t dequeue(Queue * f)
{
    uint16_t aux;
    if(!queueIsEmpty(f))
    {
        aux = f->queueItem[f->head];
        f->head = (f->head + 1)%f->max;
        return aux;
    }

    return (uint16_t)65535; /* 2^16 -1*/

}

