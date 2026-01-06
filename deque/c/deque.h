#ifndef DEQUE_H
#define DEQUE_H

/*
 * Deque (double-ended queue) of integers.
 *
 * This interface exposes a deque implemented as a circular buffer.
 * The buffer grows dynamically (typically doubling capacity) when full.
 *
 * Invariants to keep in mind while implementing:
 * - size is the number of valid elements stored in the deque.
 * - capacity is the total number of slots in the allocated array.
 * - head is the index of the current front element (if size > 0).
 * - tail is the index where the next push_back will write.
 * - When size == 0, head and tail are equal (implementation choice).
 *
 * The logical order of elements is:
 *   data[head], data[(head+1) % capacity], ... for size elements.
 *
 * NOTE: All functions assume the pointer to Deque is non-NULL.
 */

typedef struct {
	int *data;     /* Dynamic array holding the elements */
	int size;      /* Number of elements currently stored */
	int capacity;  /* Allocated size of data[] */
	int head;      /* Index of the front element */
	int tail;      /* Index where the next back insertion occurs */
} Deque;

/*
 * Initialize an empty deque with a given initial capacity.
 *
 * - initial_capacity must be >= 1.
 * - On success, returns 0 and sets size = 0, head = 0, tail = 0.
 * - On failure (invalid capacity or allocation failure), returns non-zero
 *   and leaves the deque in a safe, empty state (data = NULL, size = 0).
 */
int deque_init(Deque *dq, int initial_capacity);

/*
 * Free the internal buffer and reset the deque to an empty state.
 *
 * After this call, dq->data is NULL and size/capacity/head/tail are 0.
 * Safe to call on an already-empty deque.
 */
void deque_destroy(Deque *dq);

/*
 * Return 1 if the deque is empty, 0 otherwise.
 */
int deque_is_empty(const Deque *dq);

/*
 * Return the number of elements currently stored in the deque.
 *
 * This is a convenience accessor; it should just return dq->size.
 */
int deque_size(const Deque *dq);

/*
 * Insert value at the back (tail) of the deque.
 *
 * - If the buffer is full, it should be resized before insertion.
 * - Returns 0 on success.
 * - Returns non-zero on allocation failure (resize failure).
 */
int deque_push_back(Deque *dq, int value);

/*
 * Insert value at the front (head) of the deque.
 *
 * - If the buffer is full, it should be resized before insertion.
 * - Returns 0 on success.
 * - Returns non-zero on allocation failure (resize failure).
 */
int deque_push_front(Deque *dq, int value);

/*
 * Remove and return the front element.
 *
 * - If the deque is empty, returns non-zero and leaves out_value unchanged.
 * - On success, writes the removed value to *out_value and returns 0.
 */
int deque_pop_front(Deque *dq, int *out_value);

/*
 * Remove and return the back element.
 *
 * - If the deque is empty, returns non-zero and leaves out_value unchanged.
 * - On success, writes the removed value to *out_value and returns 0.
 */
int deque_pop_back(Deque *dq, int *out_value);

/*
 * Read (but do not remove) the front element.
 *
 * - If empty, returns non-zero and leaves out_value unchanged.
 * - On success, writes the front value to *out_value and returns 0.
 */
int deque_front(const Deque *dq, int *out_value);

/*
 * Read (but do not remove) the back element.
 *
 * - If empty, returns non-zero and leaves out_value unchanged.
 * - On success, writes the back value to *out_value and returns 0.
 */
int deque_back(const Deque *dq, int *out_value);

#endif
