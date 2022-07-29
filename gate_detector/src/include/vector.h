#ifndef __VECTOR__
#define __VECTOR__
#include <stdint.h>
#define VECTOR_INIT_CAPACITY 4

struct Vector {
    uint16_t *items;
    uint16_t capacity;
    uint16_t total;
}__attribute__((packed));

typedef struct Vector Vector;

void vector_init(Vector *);
uint16_t vector_total(Vector *);
void vector_add(Vector *, uint16_t);
void vector_set(Vector *, uint16_t, uint16_t);
uint16_t vector_get(Vector *, uint16_t);
void vector_delete(Vector *, uint16_t);
void vector_free(Vector *);

#endif
