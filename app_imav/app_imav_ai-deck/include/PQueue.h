#ifndef __PQueue__
#define __PQueue__
#include <stdint.h>
#include <stdbool.h>
#include "imageIO.h"

struct PQueue
{
    int data;
    int priority;
    struct PQueue* next;
};

typedef struct PQueue PQueue;

PQueue* newPQueue(int data, int priority);
int peek(PQueue** head);
void pop(PQueue** head); 
void push(PQueue** head, int d, int p);
int isEmpty(PQueue** head);
#endif
