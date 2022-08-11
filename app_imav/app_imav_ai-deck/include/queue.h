#ifndef __QUEUE__
#define __QUEUE__

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

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

PQueue * createPQueue();
void destroyPQueue(PQueue*);
bool pQueueIsEmpty(PQueue*);
PQueueNode pQueuePeek(PQueue*);
void pEnqueue(PQueue*, PQueueNode);
PQueueNode pDequeue(PQueue*);

#endif
