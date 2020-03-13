#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>
#include "dbg.h"
#include "cqcq.h"

static void test_add_golden(void **state) {
	int* test_value = malloc(sizeof(int));
	*test_value = 34;
	CqQueue* cq = queue_create();
	cq = queue_add(cq, test_value);
	int size = queue_size(cq);
	assert_int_equal(size, 1);

}

/**
 * @brief Given we add one element to the queue with a value, verify that value is retrieved when we pop
 */
static void test_add_golden_check_value(void **state) {
	int* test_value = malloc(sizeof(int));
	*test_value = 34;

	int* test_value2 = malloc(sizeof(int));
	*test_value2 = 44;

	CqQueue* q = queue_create();
	q = queue_add(q, test_value);
	q = queue_add(q, test_value);
	int* observed = (int*)queue_pop(q);
	assert_int_equal(34, *observed);
}


void* bad_malloc(size_t size) {
	errno = ENOMEM;
	return NULL;
}

/**
 * @brief Given that we cannot allocate memory for another element on the queue, verify that queue is NULL
 */
static void test_queue_add_no_memory(void** state) {
	int* test_value = malloc(sizeof(int));
	*test_value = 34;
	CqQueue* q = queue_create();

	q = _queue_add(q, test_value, bad_malloc);
	assert_false(q->success);
}

/**
 * @brief Given that we cannot allocate memory for a new queue, verify that queue is null
 */
static void test_queue_create_no_memory(void** state) {
	int* test_value = malloc(sizeof(int));
	*test_value = 34;
	CqQueue* q = _queue_create(bad_malloc);
	assert_null(q);
}

/**
 * @brief Given that we have a queue with three elements, verify that we pop the last one added */

static void test_pop_simple(void** state) {
	CqQueue* q = queue_create();
	int value1 = 1;
	int value2 = 2;
	int value3 = 3;
	q = queue_add(q, &value1);
	q = queue_add(q, &value2);
	q = queue_add(q, &value3);
	
	int* result;
	result = (int*)queue_pop(q);

	assert_int_equal(3, *result);
	assert_int_equal(2, *result);
	assert_int_equal(1, *result);
}

/**
 * @brief Given an empty queue, verify that queue_size is zero
 */
static void test_empty_queue_zero_size(void** state) {
	CqQueue* q = queue_create();
	assert_int_equal(0, queue_size(q));

}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_add_golden),
		cmocka_unit_test(test_add_golden_check_value),
		cmocka_unit_test(test_queue_add_no_memory),
		cmocka_unit_test(test_queue_create_no_memory),
		cmocka_unit_test(test_pop_simple),
		cmocka_unit_test(test_empty_queue_zero_size),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
}
