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
    const int values[] = {21, 34, 55, 89};
    int_ring_init(&ring);

    for (size_t item_index = 0; item_index < sizeof(values) / sizeof(values[0]); item_index++) {
        TEST_ASSERT_TRUE(int_ring_push(&ring, values[item_index]));
    }

    for (size_t item_index = 0; item_index < sizeof(values) / sizeof(values[0]); item_index++) {
        TEST_ASSERT_TRUE(int_ring_pop(&ring, &value));
        TEST_ASSERT_EQUAL_INT(values[item_index], value);
    }

    TEST_ASSERT_TRUE(int_ring_is_empty(&ring));
}

// Test 4: Filling the buffer exactly to capacity
// (Check state when 8 items are stored)

void test_filling_buffer_exactly_to_capacity(void){
    IntRing ring;
    int_ring_init(&ring);

    for (size_t item_index = 0; item_index < INT_RING_CAPACITY; item_index++) {
        TEST_ASSERT_TRUE(int_ring_push(&ring, (int)item_index));
    }

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

    // Fill the buffer to capacity.
    for (size_t item_index = 0; item_index < INT_RING_CAPACITY; item_index++) {
        TEST_ASSERT_TRUE(int_ring_push(&ring, 10 + (int)item_index));
    }

    size_t head_before = ring.head;
    size_t tail_before = ring.tail;
    size_t size_before = ring.size;

    TEST_ASSERT_FALSE(int_ring_push(&ring, 999));

    // Verify the rejected push did not change the buffer's indices or size.
    TEST_ASSERT_EQUAL_UINT(head_before, ring.head);
    TEST_ASSERT_EQUAL_UINT(tail_before, ring.tail);
    TEST_ASSERT_EQUAL_UINT(size_before, ring.size);
    // Confirm the rejected push did not change any stored values.
    for (size_t item_index = 0; item_index < INT_RING_CAPACITY; item_index++) {
        TEST_ASSERT_EQUAL_INT(
            10 + (int)item_index,
            ring.data[item_index]
        );
    }
}

// Test 6: Rejection of a pop and peek when empty
// (Check pop and peek return false and output pointer is not modified)

void test_pop_and_peek_rejected_when_empty(void){
    IntRing ring;
    int value = 123;
    int_ring_init(&ring);

    TEST_ASSERT_FALSE(int_ring_pop(&ring, &value));
    TEST_ASSERT_EQUAL_INT(123, value);

    TEST_ASSERT_FALSE(int_ring_peek(&ring, &value));
    TEST_ASSERT_EQUAL_INT(123, value);

    TEST_ASSERT_TRUE(int_ring_is_empty(&ring));
    TEST_ASSERT_EQUAL_UINT(0, int_ring_size(&ring));
}

// Test 7: At least two complete wrap-around cycles
// (Verify head and tail wrap around index boundary correctly multiple times)
void test_at_least_two_wrap_around_cycles(void) {
    IntRing ring;
    int value = 0;
    int_ring_init(&ring);

    for (size_t iteration = 0; iteration < 2U * INT_RING_CAPACITY; iteration++) {
        TEST_ASSERT_TRUE(int_ring_push(&ring, (int)iteration));

        TEST_ASSERT_EQUAL_UINT(iteration % INT_RING_CAPACITY, ring.head);
        // After the push, tail advances to the next slot, wrapping at capacity.
        TEST_ASSERT_EQUAL_UINT(
            (iteration + 1U) % INT_RING_CAPACITY,
            ring.tail
        );

        TEST_ASSERT_TRUE(int_ring_pop(&ring, &value));
        TEST_ASSERT_EQUAL_INT((int)iteration, value);

        TEST_ASSERT_EQUAL_UINT(
            (iteration + 1U) % INT_RING_CAPACITY,
            ring.head
        );
        TEST_ASSERT_EQUAL_UINT(ring.head, ring.tail);
        TEST_ASSERT_EQUAL_UINT(0, int_ring_size(&ring));
    }

    TEST_ASSERT_TRUE(int_ring_is_empty(&ring));
    TEST_ASSERT_EQUAL_UINT(0, ring.head);
    TEST_ASSERT_EQUAL_UINT(0, ring.tail);
}

// Test 8: Reuse after the buffer has become empty again
// (Ensure the buffer remains fully functional across multiple empty/refill cycles)
void test_reuse_after_buffer_becomes_empty(void){
    IntRing ring;
    int value = 0;
    int_ring_init(&ring);

    for (size_t cycle = 0; cycle < 3U; cycle++) {
        TEST_ASSERT_TRUE(int_ring_is_empty(&ring));

        for (size_t item_index = 0; item_index < INT_RING_CAPACITY; item_index++) {
            int expected = (int)(cycle * INT_RING_CAPACITY + item_index);
            TEST_ASSERT_TRUE(int_ring_push(&ring, expected));
        }

        TEST_ASSERT_TRUE(int_ring_is_full(&ring));
        TEST_ASSERT_EQUAL_UINT(INT_RING_CAPACITY, int_ring_size(&ring));

        for (size_t item_index = 0; item_index < INT_RING_CAPACITY; item_index++) {
            int expected = (int)(cycle * INT_RING_CAPACITY + item_index);
            TEST_ASSERT_TRUE(int_ring_pop(&ring, &value));
            TEST_ASSERT_EQUAL_INT(expected, value);
        }

        TEST_ASSERT_TRUE(int_ring_is_empty(&ring));
        TEST_ASSERT_EQUAL_UINT(0, int_ring_size(&ring));
    }
}


int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_newly_initialized_buffer);
    RUN_TEST(test_one_push_one_pop);
    RUN_TEST(test_fifo_ordering_of_several_values);
    RUN_TEST(test_filling_buffer_exactly_to_capacity);
    RUN_TEST(test_push_rejected_when_buffer_is_full);
    RUN_TEST(test_pop_and_peek_rejected_when_empty);
    RUN_TEST(test_at_least_two_wrap_around_cycles);
    RUN_TEST(test_reuse_after_buffer_becomes_empty);

    return UNITY_END();
}