/*
 * Sorting module - public interface and contract.
 *
 * IMPORTANT: This header is intentionally verbose. It provides the full
 * contract that every sorting algorithm in this module MUST respect, plus
 * algorithm-specific notes and a brief plain-language explanation of how each
 * algorithm works. The goal is to make the rules and expectations explicit
 * before writing any code.
 *
 * ============================================================
 * GLOBAL CONTRACT (applies to ALL algorithms below)
 * ============================================================
 *
 * 1) Input and output
 *    - Input:
 *      - TaggedInt *a : pointer to the first element of an array of TaggedInt.
 *      - int n        : number of elements in the array (n >= 0).
 *    - Output:
 *      - All algorithms sort the array IN PLACE in non-decreasing order
 *        using the field a[i].key as the sorting key.
 *      - No function returns a new array; the input array is modified.
 *      - The multiset of keys is preserved: no element is created or lost.
 *
 *    WHY TaggedInt?
 *      - We want to TEST stability, which requires distinguishing duplicates.
 *      - The field key is the value to sort by.
 *      - The field id is a stable tag (original position) used to verify
 *        whether equal keys keep their relative order.
 *
 * 2) Pre-conditions and validity
 *    - If n <= 1, the function does nothing (array is already sorted).
 *    - If n > 0, the pointer a must be non-NULL and point to at least n items.
 *    - This module assumes valid inputs. Passing NULL with n > 0 is undefined.
 *
 * 3) Correctness criteria
 *    - After completion:
 *      - a[0].key <= a[1].key <= ... <= a[n-1].key
 *      - The output is a permutation of the input (same items, same counts).
 *    - There must be no out-of-bounds access.
 *
 * 4) Stability (relative order of equal elements)
 *    - Some algorithms are stable by nature; others are not.
 *    - Because each item carries an id, tests can verify that for equal
 *      keys the id order is preserved (stable) or not (unstable).
 *    - If stability matters, read the algorithm-specific notes carefully.
 *
 * 5) Complexity expectations (high-level)
 *    - Insertion sort, selection sort: O(n^2) time, O(1) extra space.
 *    - Merge sort: O(n log n) time, O(n) extra space, stable.
 *    - Quick sort: average O(n log n), worst O(n^2), in-place, not stable.
 *    - Heap sort: O(n log n) time, in-place, not stable.
 *
 * ============================================================
 * WHY A SINGLE FILE?
 * ============================================================
 * The implementations will live in a single sort.c to allow easy comparison
 * between algorithms and to share small helper functions (swap, partition,
 * merge buffers) without spreading code across many files.
 */

#ifndef SORT_H
#define SORT_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct TaggedInt {
	int key;
	int id;
} TaggedInt;



/*
 * insertion_sort
 * ------------------------------------------------------------
 * HOW IT WORKS (high-level):
 *   Builds a sorted prefix one element at a time. At step i, it takes
 *   a[i] and "inserts" it into the correct position within the already
 *   sorted subarray a[0..i-1] by shifting larger elements to the right.
 *
 * SPECIFICS:
 *   - Stable: YES (equal elements keep their relative order).
 *   - Best case: O(n) when the array is already sorted (few shifts).
 *   - Worst case: O(n^2) when the array is in descending order.
 *   - In-place: YES (only a constant number of extra variables).
 *
 * INPUT/OUTPUT:
 *   - Input:  a (array of TaggedInt), n (length).
 *   - Output: a is sorted by key in non-decreasing order, in place.
 */
void insertion_sort(TaggedInt *a, int n);

/*
 * selection_sort
 * ------------------------------------------------------------
 * HOW IT WORKS (high-level):
 *   Repeatedly selects the smallest element from the unsorted suffix
 *   and swaps it into the next position of the sorted prefix. After i
 *   iterations, a[0..i-1] contains the i smallest elements in order.
 *
 * SPECIFICS:
 *   - Stable: NO (swaps can reorder equal elements).
 *   - Time: O(n^2) in all cases (always scans the remaining suffix).
 *   - In-place: YES (only a constant number of extra variables).
 *
 * INPUT/OUTPUT:
 *   - Input:  a (array of TaggedInt), n (length).
 *   - Output: a is sorted by key in non-decreasing order, in place.
 */
void selection_sort(TaggedInt *a, int n);

/*
 * merge_sort
 * ------------------------------------------------------------
 * HOW IT WORKS (high-level):
 *   Divide the array into two halves, recursively sort each half, then
 *   merge the two sorted halves into a single sorted array. The merge
 *   step compares the heads of both halves and copies the smaller one
 *   into a temporary buffer, preserving order for equal elements.
 *
 * SPECIFICS:
 *   - Stable: YES (merge preserves the order of equal elements).
 *   - Time: O(n log n) in all cases.
 *   - Extra space: O(n) temporary buffer for merging.
 *
 * INPUT/OUTPUT:
 *   - Input:  a (array of TaggedInt), n (length).
 *   - Output: a is sorted by key in non-decreasing order, in place.
 *
 * NOTES ON MEMORY:
 *   - A typical implementation allocates a temporary buffer of size n.
 *   - If allocation fails, the implementation must choose a behavior
 *     (e.g., leave the array unchanged and return, or abort). Because
 *     this function returns void, that behavior must be documented in
 *     the implementation source.
 */
void merge_sort(TaggedInt *a, int n);

/*
 * quick_sort
 * ------------------------------------------------------------
 * HOW IT WORKS (high-level):
 *   Picks a pivot element, partitions the array so that elements smaller
 *   than the pivot go to its left and larger ones to its right, then
 *   recursively sorts the two partitions. Good pivot choices lead to
 *   balanced partitions and fast performance.
 *
 * SPECIFICS:
 *   - Stable: NO (partitioning can reorder equal elements).
 *   - Average time: O(n log n).
 *   - Worst case: O(n^2) if partitions are highly unbalanced.
 *   - In-place: YES (partitioning rearranges elements within the array).
 *
 * INPUT/OUTPUT:
 *   - Input:  a (array of TaggedInt), n (length).
 *   - Output: a is sorted by key in non-decreasing order, in place.
 *
 * PIVOT CHOICE:
 *   - A naive pivot (first/last element) can degrade on sorted input.
 *   - Common robust choices: median-of-three or random pivot.
 */
void quick_sort(TaggedInt *a, int n);

/*
 * heap_sort
 * ------------------------------------------------------------
 * HOW IT WORKS (high-level):
 *   Builds a binary heap (max-heap for ascending sort). Then repeatedly
 *   swaps the root (maximum) with the last element of the heap, reduces
 *   the heap size, and restores the heap property (heapify). The array
 *   ends up sorted because the largest elements are placed at the end.
 *
 * SPECIFICS:
 *   - Stable: NO (heap operations can reorder equal elements).
 *   - Time: O(n log n) in all cases.
 *   - In-place: YES (heap stored in the array itself).
 *
 * INPUT/OUTPUT:
 *   - Input:  a (array of TaggedInt), n (length).
 *   - Output: a is sorted by key in non-decreasing order, in place.
 */
void heap_sort(TaggedInt *a, int n);

void print_array(TaggedInt *a, int n);

#ifdef __cplusplus
}
#endif

#endif /* SORT_H */
