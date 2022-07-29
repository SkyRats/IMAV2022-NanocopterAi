#ifndef __QUEUE__
#define __QUEUE__
#include <stdint.h>
#include <stdbool.h>

struct Queue
{
    uint16_t *queueItem;
    uint16_t head;
    uint16_t tail;
    uint16_t max;
}__attribute__((packed));

typedef struct Queue Queue;

Queue * createQueue(uint16_t size);
void destroyQueue(Queue*);
bool queueIsEmpty(Queue*);
void increaseQueue(Queue*);
uint16_t queuePeek(Queue*);
void enqueue(Queue*, uint16_t);
uint16_t dequeue(Queue*);


#endif
