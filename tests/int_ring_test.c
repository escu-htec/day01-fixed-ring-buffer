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
    const int values[] = {21, 34, 55, 89};
    int value = 0;
    int_ring_init(&ring);

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        TEST_ASSERT_TRUE(int_ring_push(&ring, values[i]));
    }

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        TEST_ASSERT_TRUE(int_ring_pop(&ring, &value));
        TEST_ASSERT_EQUAL_INT(values[i], value);
    }

    TEST_ASSERT_TRUE(int_ring_is_empty(&ring));
}

// Test 4: Filling the buffer exactly to capacity
// (Check state when 8 items are stored)

void test_filling_buffer_exactly_to_capacity(void){
    IntRing ring;
    int_ring_init(&ring);

    for (size_t i = 0; i < INT_RING_CAPACITY; i++) {
        TEST_ASSERT_TRUE(int_ring_push(&ring, (int)i));
    }

    TEST_ASSERT_EQUAL_UINT(INT_RING_CAPACITY, int_ring_size(&ring));
    TEST_ASSERT_TRUE(int_ring_is_full(&ring));
    TEST_ASSERT_FALSE(int_ring_is_empty(&ring));
    TEST_ASSERT_EQUAL_UINT(0, ring.head);
    TEST_ASSERT_EQUAL_UINT(0, ring.tail);
}

// Test 5: Rejection of a push when full
// (Check push returns false and buffer is not modified)

void test_push_rejected_when_buffer_is_full(void){
    IntRing ring;
    int data_before[INT_RING_CAPACITY];
    int_ring_init(&ring);

    for (size_t i = 0; i < INT_RING_CAPACITY; i++) {
        TEST_ASSERT_TRUE(int_ring_push(&ring, (int)i + 10));
        data_before[i] = ring.data[i];
    }

    size_t head_before = ring.head;
    size_t tail_before = ring.tail;
    size_t size_before = ring.size;

    TEST_ASSERT_FALSE(int_ring_push(&ring, 999));

    TEST_ASSERT_EQUAL_UINT(head_before, ring.head);
    TEST_ASSERT_EQUAL_UINT(tail_before, ring.tail);
    TEST_ASSERT_EQUAL_UINT(size_before, ring.size);
    for (size_t i = 0; i < INT_RING_CAPACITY; i++) {
        TEST_ASSERT_EQUAL_INT(data_before[i], ring.data[i]);
    }
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