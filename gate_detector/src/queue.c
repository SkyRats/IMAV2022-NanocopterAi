#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

Queue * createQueue(uint16_t size)
{
    Queue *q = NULL;
    q = malloc(sizeof(Queue));
    if(q == NULL)
    {
        #ifdef DEBUG_ON
        printf("Allocation of memory failed on createQueue.\n");
        #endif
        return NULL;
    }
    q->queueItem = malloc(size * sizeof(uint16_t));
    q->max = size;
    q->head = 0;
    q->tail = 0;
    return q;
}

void destroyQueue(Queue * q)
{
    free(q->queueItem);
    free(q);
}

bool queueIsEmpty(Queue * q)
{
    return (q->head == q->tail);
}

void increaseQueue(Queue * q)
{
    uint16_t *v2 = malloc(2 * q->max * sizeof(uint16_t));
    if(v2 == NULL)
    {
        #ifdef DEBUG_ON
        printf("Allocation of memory failed on increaseQueue\n");
        #endif
        return;
    }

    #ifdef DEBUG_ON
    printf("Queue's size was increased to %d\n", 2*q->max*sizeof(uint16_t));
    #endif
    int i, k;

    for(i = q->head, k = 0; i != q->tail; i = (i + 1)%q->max)
        v2[k] = q->queueItem[i];

    free(q->queueItem);

    q->head = 0;
    q->tail = q->max - 1;
    q->max *= 2;
    q->queueItem = v2;
}

uint16_t queuePeek(Queue * q)
{
    if(!queueIsEmpty(q))
        return q->queueItem[q->head];
    return (uint16_t)65535; /* 2^16 -1*/
}

void enqueue(Queue * q, uint16_t X)
{
    if(q->head == (q->tail + 1)%q->max) /* Queue is full */
        increaseQueue(q);

    q->queueItem[q->tail] = X;
    q->tail = (q->tail + 1)%q->max;
}

uint16_t dequeue(Queue * q)
{
    uint16_t aux;
    if(!queueIsEmpty(q))
    {
        aux = q->queueItem[q->head];
        q->head = (q->head + 1)%q->max;
        return aux;
    }

    return (uint16_t)65535; /* 2^16 -1*/

}

