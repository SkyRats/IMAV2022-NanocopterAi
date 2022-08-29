#include "pmsis.h"
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

PQueue * createPQueue()
{
    //pi_cl_alloc_req_t alloc_req;

    //pi_cl_l2_malloc(sizeof(PQueue), &alloc_req);
    //PQueue *pq = pi_cl_l2_malloc_wait(&alloc_req);
    PQueue *pq = pmsis_l1_malloc(sizeof(PQueue));

    if(pq == NULL)
    {
        #ifdef DEBUG_ON
        printf("Allocation of memory failed on createPQueue.\n");
        #endif
        return NULL;
    }

    memset(pq, 0, sizeof(PQueue));

    return pq;
}

void destroyPQueue(PQueue* pq)
{
    //pi_cl_free_req_t free_req;

    PQueueNode* pqn = pq->pQueueItem, *pqnn;

    //pi_cl_l2_free(pq, sizeof(PQueue), &free_req);
    //pi_cl_l2_free_wait(&free_req);
    pmsis_l1_malloc_free(pq, sizeof(PQueue));

    while(pqn != NULL)
    {
        //pi_cl_free_req_t free_reqn;

        pqnn = pqn->nextNode;

        //pi_cl_l2_free(pqn, sizeof(PQueueNode), &free_reqn);
        //pi_cl_l2_free_wait(&free_reqn);
	pmsis_l1_malloc_free(pqn, sizeof(PQueueNode));

        pqn = pqnn;
    }
}

bool pQueueIsEmpty(PQueue* pq)
{
    return pq->size == 0;
}

PQueueNode pQueuePeek(PQueue* pq)
{
    return *(pq->pQueueItem);
}

void pEnqueue(PQueue* pq, PQueueNode pqi)
{
    //pi_cl_alloc_req_t alloc_req;

    //pi_cl_l2_malloc(sizeof(PQueueNode), &alloc_req);
    //PQueueNode* newNode = pi_cl_l2_malloc_wait(&alloc_req);
    PQueueNode * newNode = pmsis_l1_malloc(sizeof(PQueueNode));
    if(newNode == NULL)
    {
        #ifdef DEBUG_ON
        printf("Allocation of memory failed on pEnqueue\n");
        #endif
        return;
    }

    /* copy the contents of the new item to the new node */
    *(newNode) = pqi;

    if(pq->size == 0 || pq->tail->priority < pqi.priority)
    {

        newNode->nextNode = NULL;

        if(pq->size == 0)
            pq->pQueueItem = newNode;
        else
            pq->tail->nextNode = newNode;

        pq->tail = newNode;

    }
    else
    {
        PQueueNode** pqn = &(pq->pQueueItem);

        while((*pqn)->priority < pqi.priority)
            pqn = &((*pqn)->nextNode);

        newNode->nextNode = *pqn;
        *pqn = newNode;
    }

    pq->size += 1;
}

PQueueNode pDequeue(PQueue* pq)
{
    if(pq->size == 0)
    {
        #ifdef DEBUG_ON
        printf("Cannot dequeue empty priority queue. \n");
        #endif
        return (PQueueNode){-1, -1, NULL};
    }

    PQueueNode* pqn = pq->pQueueItem;
    pq->pQueueItem = pq->pQueueItem->nextNode;

    PQueueNode pqi = *pqn;

    //pi_cl_free_req_t free_req;

    //pi_cl_l2_free(pqn, sizeof(PQueueNode), &free_req);
    //pi_cl_l2_free_wait(&free_req);
    
    pmsis_l1_malloc_free(pqn, sizeof(PQueueNode));

    pq->size -= 1;

    return pqi;
}
