#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "config.h"

#ifndef STACK_INITIAL_SIZE
#define STACK_INITIAL_SIZE 100
#endif

struct Stack
{
    uint16_t *item;
    uint16_t top;
    uint16_t max;

}__attribute__((packed));

typedef struct Stack Stack;

Stack * createStack();
void reallocStack(Stack *);
uint16_t pop(Stack *);
uint16_t topOfStack(Stack *);
void destroyStack(Stack *);
void push(Stack *, uint16_t);
bool isStackEmpty(Stack *);

#endif
