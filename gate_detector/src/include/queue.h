#ifndef __QUEUE__
#define __QUEUE__
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t *queueItem;
    uint32_t head;
    uint32_t tail;
    uint32_t max;
} Queue;

Queue * createQueue(uint32_t size);
void destroyQueue(Queue*);
bool queueIsEmpty(Queue*);
void increaseQueue(Queue*);
uint16_t queuePeek(Queue*);
void enqueue(Queue*, uint16_t);
uint16_t dequeue(Queue*);


#endif
