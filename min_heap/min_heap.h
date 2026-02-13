#ifndef MIN_HEAP_H
#define MIN_HEAP_H

/*
 * Min-Heap (Priority Queue) for integer keys.
 *
 * This header is intentionally detailed: it defines the data structure,
 * the exact behavior of each operation, edge cases, invariants, and
 * complexity expectations.
 *
 * Key policy:
 * - Duplicates are allowed. Each insert adds a new element.
 *
 * Invariants:
 * - data is a dynamic array representing a complete binary tree.
 * - size is the number of elements currently stored (0 <= size <= capacity).
 * - Heap property (min-heap):
 *   For every valid index i, data[i] <= data[left(i)] and data[i] <= data[right(i)]
 *   when those child indices are within [0, size).
 *
 * Array index mapping (0-based):
 * - parent(i) = (i - 1) / 2  for i > 0
 * - left(i)   = 2*i + 1
 * - right(i)  = 2*i + 2
 *
 * Complexity (n = number of elements):
 * - peek: O(1)
 * - insert (push): O(log n)
 * - extract_min: O(log n)
 * - build/heapify from array: O(n)
 *
 * Preconditions:
 * - All functions except min_heap_init assume the heap has been initialized
 *   via min_heap_init. Calling them on an uninitialized heap is undefined
 *   behavior.
 *
 * Return codes (when applicable):
 * - 0  : success
 * - -1 : allocation failure
 * - 1  : other errors (e.g., empty heap, invalid arguments)
 */

#include <stddef.h>

#define MIN_HEAP_DEFAULT_CAPACITY 4

typedef struct {
	int *data;
	int size;
	int capacity;
} MinHeap;

/*
 * Initialize an empty heap with the given initial capacity.
 *
 * If initial_capacity < 1, the implementation should treat it as 1.
 *
 * Returns:
 * - 0 on success
 * - -1 on allocation failure
 *
 * Postconditions on success:
 * - heap->data != NULL
 * - heap->size == 0
 * - heap->capacity >= 1
 */
int min_heap_init(MinHeap *heap, int initial_capacity);

/*
 * Destroy the heap and free its internal array.
 *
 * Postconditions:
 * - heap->data == NULL
 * - heap->size == 0
 * - heap->capacity == 0
 *
 * Safe to call on an already-empty heap (size == 0),
 * provided the heap was initialized.
 */
void min_heap_destroy(MinHeap *heap);

/*
 * Return 1 if the heap is empty, 0 otherwise.
 */
int min_heap_is_empty(const MinHeap *heap);

/*
 * Return the number of elements stored in the heap.
 */
int min_heap_size(const MinHeap *heap);

/*
 * Insert a value into the heap.
 *
 * Behavior:
 * - The value is appended at the end, then sifted up until the heap
 *   property is restored.
 * - If the internal array is full, it is resized (typically doubled).
 *
 * Returns:
 * - 0 on success
 * - -1 on allocation failure (heap unchanged)
 */
int min_heap_push(MinHeap *heap, int value);

/*
 * Return (without removing) the minimum value.
 *
 * Returns:
 * - 0 on success (writes result to *out_value)
 * - 1 if the heap is empty (out_value unchanged)
 */
int min_heap_peek(const MinHeap *heap, int *out_value);

/*
 * Remove and return the minimum value.
 *
 * Behavior:
 * - The root value is saved.
 * - The last element is moved to the root, size is decreased,
 *   then sifted down until the heap property is restored.
 *
 * Returns:
 * - 0 on success (writes result to *out_value)
 * - 1 if the heap is empty (out_value unchanged)
 */
int min_heap_extract_min(MinHeap *heap, int *out_value);

/*
 * Optional helper: build a heap from an array of values.
 *
 * Behavior:
 * - The heap's internal array is replaced (or reused if large enough).
 * - The values are copied, then heapified in O(n).
 *
 * Precondition:
 * - heap has been initialized (min_heap_init).
 * - values is non-NULL and n >= 0.
 *
 * Returns:
 * - 0 on success
 * - 1 on invalid arguments (heap unchanged)
 * - -1 on allocation failure (heap unchanged)
 */
int min_heap_build(MinHeap *heap, const int *values, int n);

#endif
