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

struct PQueueNode
{
    uint16_t pQueueItem;
    uint16_t priority;
    struct PQueueNode* nextNode;
}__attribute__((packed));

typedef struct PQueueNode PQueueNode;


struct PQueue
{
    PQueueNode *pQueueItem;
    PQueueNode *tail;
    uint16_t size;
}__attribute__((packed));

typedef struct PQueue PQueue;

Queue * createQueue(uint16_t size);
void destroyQueue(Queue*);
bool queueIsEmpty(Queue*);
void increaseQueue(Queue*);
uint16_t queuePeek(Queue*);
void enqueue(Queue*, uint16_t);
uint16_t dequeue(Queue*);

PQueue * createPQueue();
void destroyPQueue(PQueue*);
bool pQueueIsEmpty(PQueue*);
PQueueNode pQueuePeek(PQueue*);
void pEnqueue(PQueue*, PQueueNode);
PQueueNode pDequeue(PQueue*);

#endif
