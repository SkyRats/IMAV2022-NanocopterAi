#include "pmsis.h"
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Stack* createStack()
{
    pi_cl_alloc_req_t alloc_req, alloc_reqn;
    Stack* s;

    pi_cl_l2_malloc(sizeof(Stack), &alloc_req);
    s = pi_cl_l2_malloc_wait(&alloc_req);

    s->max = STACK_INITIAL_SIZE;
    s->top = 0;

    pi_cl_l2_malloc(STACK_INITIAL_SIZE*sizeof(uint16_t), &alloc_reqn);
    s->item = pi_cl_l2_malloc_wait(&alloc_reqn);

    return s;
}

void destroyStack(Stack * s)
{
    pi_cl_free_req_t free_req, free_reqn;

    pi_cl_l2_free(s->item, (s->max)*sizeof(uint16_t), &free_req);
    pi_cl_l2_free_wait(&free_req);

    pi_cl_l2_free(s, sizeof(Stack), &free_reqn);
    pi_cl_l2_free_wait(&free_reqn);
}

void push(Stack * s, uint16_t i)
{
    if(s->top == s->max)
    {
        pi_cl_alloc_req_t alloc_req;
        pi_cl_free_req_t free_req;
        uint16_t previousSize = s->max;
        uint16_t * newItemList;
        pi_cl_l2_malloc((previousSize + STACK_INITIAL_SIZE)*sizeof(uint16_t), &alloc_req);
        newItemList = pi_cl_l2_malloc_wait(&alloc_req);

        if(newItemList != NULL)
            for(uint16_t i = 0; i < previousSize; ++i)
                newItemList[i] = s->item[i];
        else
        {
            printf("allocation for new list of items failed.\n");
            return;
        }

        pi_cl_l2_free(s->item, previousSize*sizeof(uint16_t), &free_req);

        s->max += STACK_INITIAL_SIZE;

        pi_cl_l2_free_wait(&free_req);

        s->item = newItemList;
    }

    s->item[(s->top)] = i;
    s->top += 1;
}

uint16_t pop(Stack * s)
{
    if(!isStackEmpty(s))
    {
        uint16_t item = s->item[(s->top)];
        s->top -= 1;
        return item;
    }
    #ifdef DEBUG_ON
    else
        printf("Popping of Stack failed.\n");
    #endif

    return (uint16_t) -1; /* (unsigned) -1 == 2^16 - 1 (max value) */
}

bool isStackEmpty(Stack * s)
{
    return (s->top == 0);
}

uint16_t topOfStack(Stack * s)
{
    if(!isStackEmpty(s))
        return s->item[(s->top) - 1];
    #ifdef DEBUG_ON
    else
        printf("Access to the top of the Stack failed.\n");
    #endif

    return (uint16_t) -1; /* (unsigned) -1 == 2^16 - 1 (max value) */
}
