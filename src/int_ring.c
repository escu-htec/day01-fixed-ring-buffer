#include "int_ring.h"

void int_ring_init(IntRing* ring){
    ring->head = 0;
    ring->tail = 0;
    ring->size = 0;
}

bool int_ring_push(IntRing* ring, int value){
    if (int_ring_is_full(ring)) {
        return false;
    }

    ring->data[ring->tail] = value;
    ring->tail = (ring->tail + 1) % INT_RING_CAPACITY;
    ring->size++;
    return true;
}

bool int_ring_pop(IntRing* ring, int* value){
    if (int_ring_is_empty(ring)) {
        return false;
    }

    *value = ring->data[ring->head];
    ring->head = (ring->head + 1) % INT_RING_CAPACITY;
    ring->size--;
    return true;
}

bool int_ring_peek(const IntRing* ring, int* value){
    if (int_ring_is_empty(ring)) {
        return false;
    }

    *value = ring->data[ring->head];
    return true;
}

bool int_ring_is_empty(const IntRing* ring){
    return int_ring_size(ring) == 0;
}

bool int_ring_is_full(const IntRing* ring){
    return int_ring_size(ring) == INT_RING_CAPACITY;
}

size_t int_ring_size(const IntRing* ring){
    return ring->size;
}