#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

void vector_init(Vector *v)
{
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = malloc(sizeof(uint16_t) * v->capacity);
    if(v == NULL)
    {
        #ifdef DEBUG_ON
        printf("Allocation of memory failed on vector_init.\n");
        #endif
    }
}

uint16_t vector_total(Vector *v)
{
    return v->total;
}

static void vector_resize(Vector *v, uint16_t capacity)
{
    #ifdef DEBUG_ON
    printf("vector_resize: %d to %d\n", v->capacity, capacity);
    #endif

    uint16_t *items = realloc(v->items, sizeof(uint16_t) * capacity);
    if (items) {
        v->items = items;
        v->capacity = capacity;
    }
    else
    {
        #ifdef DEBUG_ON
        printf("Allocation of memory failed on vector_resize.\n");
        #endif
    }
}

void vector_add(Vector *v, uint16_t item)
{
    if (v->capacity == v->total)
        vector_resize(v, v->capacity * 2);
    v->items[(v->total)++] = item;
}

void vector_set(Vector *v, uint16_t index, uint16_t item)
{
    if (index >= 0 && index < v->total)
        v->items[index] = item;
}

uint16_t vector_get(Vector *v, uint16_t index)
{
    if (index >= 0 && index < v->total)
        return v->items[index];
    return (uint16_t)65535;
}

void vector_delete(Vector *v, uint16_t index)
{
    if (index < 0 || index >= v->total)
        return;

    v->items[index] = 0;

    for (uint16_t i = index; i < v->total - 1; i++) {
        v->items[i] = v->items[i + 1];
        v->items[i + 1] = 0;
    }

    (v->total)--;

    if (v->total > 0 && v->total == v->capacity / 4)
        vector_resize(v, v->capacity / 2);
}

void vector_free(Vector *v)
{
    free(v->items);
}
