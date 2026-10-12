#include "unity.h"
#include "int_ring.h"

void setUp(void){}

void tearDown(void){}

// Test 1: A newly initialized buffer
// (Check head, tail, size, empty, full)


// Test 2: One push and one pop
// (Check value returned and buffer state changes)

// Test 3: FIFO ordering of several values
// (Check values come out in the exact sequence they entered)

// Test 4: Filling the buffer exactly to capacity
// (Check state when 8 items are stored)

// Test 5: Rejection of a push when full
// (Check push returns false and buffer is not modified)

// Test 6: Rejection of a pop and peek when empty
// (Check pop and peek return false and output pointer is not modified)

// Test 7: At least two complete wrap-around cycles
// (Verify head and tail wrap around index boundary correctly multiple times)

// Test 8: Reuse after the buffer has become empty again
// (Ensure the buffer remains fully functional across multiple empty/refill cycles)


int main(void){
    UNITY_BEGIN();

    return UNITY_END();
}