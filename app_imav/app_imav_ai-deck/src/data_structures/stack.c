#include "pmsis.h"
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Stack* createStack()
{
    Stack* s;
    s = pmsis_l2_malloc(sizeof(Stack));
    s->max = STACK_INITIAL_SIZE;
    s->top = 0;
    s->item = pmsis_l2_malloc(STACK_INITIAL_SIZE*sizeof(uint16_t));
    return s;
}

void destroyStack(Stack * s)
{
    pmsis_l2_malloc_free(s->item, (s->max)*sizeof(uint16_t));
    pmsis_l2_malloc_free(s, sizeof(Stack));
}

Stack * reallocStack(Stack *s)
{
    uint16_t previousSize = s->max;
    Stack * newStack = pmsis_l2_malloc(sizeof(Stack));

    newStack->item = pmsis_l2_malloc((previousSize + STACK_INITIAL_SIZE)*sizeof(uint16_t));
    newStack->top = s->top;
    newStack->max = previousSize + STACK_INITIAL_SIZE;

    for(uint16_t i = 0; i < previousSize; ++i)
        newStack->item[i] = s->item[i];

    pmsis_l2_malloc_free(s->item, previousSize*sizeof(uint16_t));
    pmsis_l2_malloc_free(s, sizeof(Stack));

    return newStack;
}

void push(Stack * s, uint16_t i)
{
    if(s->top == s->max)
        s = reallocStack(s);

    s->item[(s->top)++] = i;
}

uint16_t pop(Stack * s)
{
    if(!isStackEmpty(s))
        return s->item[--(s->top)];
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
