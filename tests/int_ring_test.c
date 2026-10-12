#include "unity.h"
#include "int_ring.h"

void setUp(void){}

void tearDown(void){}

// Test 1: A newly initialized buffer
// (Check head, tail, size, empty, full)

void test_newly_initialized_buffer(void){
    IntRing ring;
    int_ring_init(&ring);

    TEST_ASSERT_EQUAL_UINT(0, ring.head);
    TEST_ASSERT_EQUAL_UINT(0, ring.tail);
    TEST_ASSERT_EQUAL_UINT(0, ring.size);

    TEST_ASSERT_TRUE(int_ring_is_empty(&ring));
    TEST_ASSERT_FALSE(int_ring_is_full(&ring));

    TEST_ASSERT_EQUAL_UINT(0, int_ring_size(&ring));
    
}


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

    RUN_TEST(test_newly_initialized_buffer);

    return UNITY_END();
}