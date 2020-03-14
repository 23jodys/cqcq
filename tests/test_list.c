#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <cmocka.h>
#include "list.h"
#include "dbg.h"


/**
 * @brief Given that we have a list with two elements and pop an element from it, verify that length is 1 and that the correct value is returned.
 */
static void test_list_pop_simple(void** state) {
	int element1 = 1;
	int element2 = 2;
	List_T* list = List_list(&element1, &element2);

	assert_int_equal(2, List_length(list));
	int* stored_value;
	list = List_pop(list, stored_value);
	assert_int_equal(1, *stored_value);
	
	int observed = List_length(list);
	assert_int_equal(1, observed);

	debug("test_list_pop_simple: %d", *(int*)stored_value);
	assert_int_equal(1, *stored_value);
	List_free(&list);
	//free(stored_value);
}

/**
 * @brief Given that we push a variety of values, verify that they pop off in LIFO order 
 */
static void test_list_push_pop(void** state) {
	int x = 1;
	int y = 2;
	int z = 3;
	List_T* test_list = List_list(NULL);

	test_list = List_push(test_list, (void*)&x);
	debug("test_list_push_pop: pointer to x is %p", &x);
	test_list = List_push(test_list, (void*)&y);
	test_list = List_push(test_list, (void*)&z);

	int* test_value; 

	test_list = List_pop(test_list, test_value);
	assert_int_equal(3, *test_value);
	test_list = List_pop(test_list, test_value);
	assert_int_equal(2, *test_value);
	test_list = List_pop(test_list, test_value);
	assert_int_equal(1, *test_value);
	assert_null(test_list);

}

/**
 * @brief Given that we request to make an empty list, verify that we don't crash
 */
static void test_list_list_empty(void** state) {
	List_T* test_list = List_list(NULL);
}

/**
 * @brief Given a two item list, verify List_length returns 2
 */
static void test_list_length_simple(void **state) {
	List_T* list = List_list("Test1", "Test2", NULL);
	int observed = List_length(list);
	assert_int_equal(2, observed);
	List_free(&list);
};

/**
 * @brief Given that we perform a bunch of push and pop, verify that length is correct
 */
static void test_list_length_complex(void** state) {
	List_T* list = List_list("Test1", "Test2");
	list = List_push(list, "Test3");
	list = List_push(list, "Test4");
	char* value;
	list = List_pop(list, value);
	int observed = List_length(list);
	debug("test_list_length_complex: %d", observed);
	assert_int_equal(3, observed);
	List_free(&list);
}

/**
 * @brief Given a NULL List_T, verify we return zero length.
 */
static void test_list_length_null(void** state) {
	assert_int_equal(0, List_length(NULL));	
}

/**
 * @brief Given that we free a list, verify that List_T now points to NULL
 */
static void test_list_free_sets_null(void** state) {
	List_T* list = List_list("a", "b", "c");	
	assert_non_null(list);
	List_free(&list);
	assert_null(list);
}

/**
 * @brief Given that we have a three item list and append an element, verify that the length is 4
*/
static void test_list_append_simple(void** state) {
	List_T* list = List_list("a", "b", "c");	
	List_T* tail = List_list("d");
	list = List_append(list, tail);

	assert_int_equal(4, List_length(list));

	List_free(&list);
}

/**
 * @brief given that we have a three element list and copy it, verify that the original still has the correct values and order and verify that the copy has the correct values and order
 */
static void test_list_copy(void** state) {
	List_T* list = List_list(INT(1), INT(2), INT(3));	
	List_T* copy = List_copy(list);

	List_T* copy_head = copy;

	for (int i = 1; i <= 3; i++) {
		debug("test_list_copy: %d, value is %d", i, *(int*)copy->value);
		assert_int_equal(i, *(int*)copy->value);
		copy = copy->tail;
	}	

	List_free(&list);
	List_free(&copy_head);
}

/**
 * @brief Given that we attempt to copy a NULL list, verify our copy is NULL
 */
static void test_list_copy_null(void** state) {
	List_T* list = NULL;
	List_T* copy;

	copy = List_copy(list);
	assert_null(copy);
}

/**
 * @brief Given that we attempt to copy a one element list, verify our copy is has the correct single value
 */
static void test_list_copy_one_element(void** state) {
	List_T* list = List_list(INT(1));
	List_T* copy;

	copy = List_copy(list);
	int* test_value; 
	copy = List_pop(copy, test_value);
	assert_int_equal(1, *test_value);

	List_free(&list);

}

/**
 * @brief Given that we attempt to copy a two element list, verify our copy is has the correct two value
 */
static void test_list_copy_two_element(void** state) {
	List_T* list = List_list(INT(1), INT(2));
	List_T* copy;

	copy = List_copy(list);
	int* test_value; 
	copy = List_pop(copy, test_value);
	assert_int_equal(1, *test_value);

	copy = List_pop(copy, test_value);
	assert_int_equal(2, *test_value);

	List_free(&list);
}

int main(int argc, char* argv[]) {



	const struct CMUnitTest test_list_copy_tests[] = {
		cmocka_unit_test(test_list_copy),
		cmocka_unit_test(test_list_copy_null),
		cmocka_unit_test(test_list_copy_one_element),
		cmocka_unit_test(test_list_copy_two_element),
	};

	const struct CMUnitTest test_list_length_tests[] = {
		cmocka_unit_test(test_list_length_simple),
		cmocka_unit_test(test_list_length_null),
		cmocka_unit_test(test_list_length_complex),
	};

	const struct CMUnitTest test_list_misc_tests[] = {
		cmocka_unit_test(test_list_free_sets_null),
		cmocka_unit_test(test_list_pop_simple),
		cmocka_unit_test(test_list_push_pop),
		cmocka_unit_test(test_list_append_simple),
	};

	const struct CMUnitTest test_list_free_tests[] = {
		cmocka_unit_test(test_list_free_sets_null),
	}; 

	const struct CMUnitTest test_list_list_tests[] = {
		cmocka_unit_test(test_list_list_empty),
	};

	char** positionals;
	bool all_tests = false;

	for (;;) {
		int opt = getopt(argc, argv, "a");
		if (opt == -1) {
			break;
		}

		if (opt == 'a') {
			all_tests = true;
		}
	}

	positionals = &argv[optind];
	bool test_list_copy = false;
	bool test_list_length = false;
	bool test_list_list = false;
	bool test_list_free = false;
	bool test_list_misc = false;
	#define set_test(name) if (strcmp(*positionals, #name) == 0) { name = true; }
	for (; *positionals; positionals++) {
		fprintf(stderr, "Positional: %s\n", *positionals);
		set_test(test_list_copy);
		set_test(test_list_length);
		set_test(test_list_list);
		set_test(test_list_free);
		set_test(test_list_misc);
	}
	#define conditional_run_tests(name) if (name == true || all_tests == true) { total_failed_tests += cmocka_run_group_tests(name ## _tests, NULL, NULL);}

	int total_failed_tests = 0;
	conditional_run_tests(test_list_copy);
	conditional_run_tests(test_list_length);
	conditional_run_tests(test_list_list);
	conditional_run_tests(test_list_free);
	conditional_run_tests(test_list_misc);

	printf("Failed tests: %d\n", total_failed_tests);

	return total_failed_tests;
}
