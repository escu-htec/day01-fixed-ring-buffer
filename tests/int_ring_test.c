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

void test_one_push_one_pop(void){
    IntRing ring;
    int value = 0;
    int_ring_init(&ring);

    TEST_ASSERT_TRUE(int_ring_push(&ring, 21));
    TEST_ASSERT_EQUAL_UINT(0, ring.head);
    TEST_ASSERT_EQUAL_UINT(1, ring.tail);
    TEST_ASSERT_EQUAL_UINT(1, int_ring_size(&ring));
    TEST_ASSERT_FALSE(int_ring_is_empty(&ring));

    TEST_ASSERT_TRUE(int_ring_pop(&ring, &value));
    TEST_ASSERT_EQUAL_INT(21, value);
    TEST_ASSERT_EQUAL_UINT(1, ring.head);
    TEST_ASSERT_EQUAL_UINT(1, ring.tail);
    TEST_ASSERT_EQUAL_UINT(0, int_ring_size(&ring));
    TEST_ASSERT_TRUE(int_ring_is_empty(&ring));
}

// Test 3: FIFO ordering of several values
// (Check values come out in the exact sequence they entered)

void test_fifo_ordering_of_several_values(void){
    IntRing ring;
    int value = 0;
    int_ring_init(&ring);

    TEST_ASSERT_TRUE(int_ring_push(&ring, 21));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 34));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 55));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 89));

    TEST_ASSERT_TRUE(int_ring_pop(&ring, &value));
    TEST_ASSERT_EQUAL_INT(21, value);
    TEST_ASSERT_TRUE(int_ring_pop(&ring, &value));
    TEST_ASSERT_EQUAL_INT(34, value);
    TEST_ASSERT_TRUE(int_ring_pop(&ring, &value));
    TEST_ASSERT_EQUAL_INT(55, value);
    TEST_ASSERT_TRUE(int_ring_pop(&ring, &value));
    TEST_ASSERT_EQUAL_INT(89, value);

    TEST_ASSERT_TRUE(int_ring_is_empty(&ring));
}

// Test 4: Filling the buffer exactly to capacity
// (Check state when 8 items are stored)

void test_filling_buffer_exactly_to_capacity(void){
    IntRing ring;
    int_ring_init(&ring);

    TEST_ASSERT_TRUE(int_ring_push(&ring, 0));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 1));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 2));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 3));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 4));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 5));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 6));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 7));

    TEST_ASSERT_EQUAL_UINT(8, int_ring_size(&ring));
    TEST_ASSERT_TRUE(int_ring_is_full(&ring));
    TEST_ASSERT_FALSE(int_ring_is_empty(&ring));
    TEST_ASSERT_EQUAL_UINT(0, ring.head);
    TEST_ASSERT_EQUAL_UINT(0, ring.tail);
}

// Test 5: Rejection of a push when full
// (Check push returns false and buffer is not modified)

void test_push_rejected_when_buffer_is_full(void){
    IntRing ring;
    int_ring_init(&ring);

    TEST_ASSERT_TRUE(int_ring_push(&ring, 10));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 11));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 12));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 13));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 14));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 15));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 16));
    TEST_ASSERT_TRUE(int_ring_push(&ring, 17));

    size_t head_before = ring.head;
    size_t tail_before = ring.tail;
    size_t size_before = ring.size;

    TEST_ASSERT_FALSE(int_ring_push(&ring, 999));

    TEST_ASSERT_EQUAL_UINT(head_before, ring.head);
    TEST_ASSERT_EQUAL_UINT(tail_before, ring.tail);
    TEST_ASSERT_EQUAL_UINT(size_before, ring.size);
    TEST_ASSERT_EQUAL_INT(10, ring.data[0]);
    TEST_ASSERT_EQUAL_INT(11, ring.data[1]);
    TEST_ASSERT_EQUAL_INT(12, ring.data[2]);
    TEST_ASSERT_EQUAL_INT(13, ring.data[3]);
    TEST_ASSERT_EQUAL_INT(14, ring.data[4]);
    TEST_ASSERT_EQUAL_INT(15, ring.data[5]);
    TEST_ASSERT_EQUAL_INT(16, ring.data[6]);
    TEST_ASSERT_EQUAL_INT(17, ring.data[7]);
}

// Test 6: Rejection of a pop and peek when empty
// (Check pop and peek return false and output pointer is not modified)

// Test 7: At least two complete wrap-around cycles
// (Verify head and tail wrap around index boundary correctly multiple times)

// Test 8: Reuse after the buffer has become empty again
// (Ensure the buffer remains fully functional across multiple empty/refill cycles)


int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_newly_initialized_buffer);
    RUN_TEST(test_one_push_one_pop);
    RUN_TEST(test_fifo_ordering_of_several_values);
    RUN_TEST(test_filling_buffer_exactly_to_capacity);
    RUN_TEST(test_push_rejected_when_buffer_is_full);

    return UNITY_END();
}