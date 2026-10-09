#include <stdbool.h>
#include <stddef.h>

#define INT_RING_CAPACITY 8U

typedef struc {
    int data[INT_RING_CAPACITY]
    size_t head;
    size_t tail;
    size_t size;
} IntRing;

void int_ring_init(IntRing* ring);
bool int_ring_push(IntRing* ring, int value);
bool int_ring_pop(IntRing* ring, int* value);
bool int_ring_peek(const IntRing* ring, int* value);
bool int_ring_is_empty(const IntRing* ring);
bool int_ring_is_full(const IntRing* ring);
size_t int_ring_size(const IntRing* ring);

