#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "deque.h"

static void check_pop_sequence(Deque *dq, const int *expected, int n) {
	int value = 0;
	for (int i = 0; i < n; i++) {
		assert(deque_pop_front(dq, &value) == 0);
		assert(value == expected[i]);
	}
}

int main(void) {
	Deque dq;
	int value = 0;

	/* Wrap-around + resize on push_back */
	assert(deque_init(&dq, 4) == 0);

	assert(deque_push_back(&dq, 1) == 0);
	assert(deque_push_back(&dq, 2) == 0);
	assert(deque_push_back(&dq, 3) == 0);
	assert(deque_push_back(&dq, 4) == 0);

	assert(deque_pop_front(&dq, &value) == 0 && value == 1);
	assert(deque_pop_front(&dq, &value) == 0 && value == 2);

	assert(deque_push_back(&dq, 5) == 0);
	assert(deque_push_back(&dq, 6) == 0);
	/* Full with head > tail now, next push triggers resize */
	assert(deque_push_back(&dq, 7) == 0);

	{
		const int expected[] = {3, 4, 5, 6, 7};
		check_pop_sequence(&dq, expected, 5);
	}
	assert(deque_is_empty(&dq));
	deque_destroy(&dq);

	/* Wrap-around + resize on push_front */
	assert(deque_init(&dq, 4) == 0);

	assert(deque_push_front(&dq, 1) == 0);
	assert(deque_push_front(&dq, 2) == 0);
	assert(deque_push_front(&dq, 3) == 0);
	assert(deque_push_front(&dq, 4) == 0);

	assert(deque_pop_back(&dq, &value) == 0 && value == 1);
	assert(deque_pop_back(&dq, &value) == 0 && value == 2);

	assert(deque_push_front(&dq, 5) == 0);
	assert(deque_push_front(&dq, 6) == 0);
	/* Full with head < tail now, next push triggers resize */
	assert(deque_push_front(&dq, 7) == 0);

	{
		const int expected[] = {7, 6, 5, 4, 3};
		check_pop_sequence(&dq, expected, 5);
	}
	assert(deque_is_empty(&dq));
	deque_destroy(&dq);

	printf("testresize: all assertions passed\n");
	return 0;
}
