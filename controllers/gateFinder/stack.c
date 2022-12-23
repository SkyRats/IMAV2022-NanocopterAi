#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Stack* createStack()
{
    Stack* s;
    s = malloc(sizeof(Stack));
    s->max = STACK_INITIAL_SIZE;
    s->top = 0;
    s->item = malloc(STACK_INITIAL_SIZE*sizeof(uint16_t));
    return s;
}

void destroyStack(Stack * s)
{
    free(s->item);
    free(s);
}

void reallocStack(Stack *s)
{
    s->item = realloc(s->item, (s->max + STACK_INITIAL_SIZE)*sizeof(uint16_t));
    s->max += STACK_INITIAL_SIZE;
}

void push(Stack * s, uint16_t i)
{
    if(s->top == s->max)
        reallocStack(s);

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
