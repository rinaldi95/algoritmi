/*
 * Reference implementations ("codex") for the sorting module.
 *
 * This file is meant as a clean, 10/10 style example. It is not intended
 * to be compiled together with sort.c because it defines the same public
 * symbols. Compile it alone if you want to test these versions.
 */
//FROM ROBERTO. I HAVE NOT TESTED THEESE FUNCTION. I JUST WANTED TO 
//SEE THE CHATGPT STYLE.
#include <stdio.h>
#include <stdlib.h>

#include "sort.h"

static void swap_items(TaggedInt *a, int i, int j) {
	TaggedInt tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

void insertion_sort(TaggedInt *a, int n) {
	for (int i = 1; i < n; i++) {
		TaggedInt key = a[i];
		int j = i;
		while (j > 0 && a[j - 1].key > key.key) {
			a[j] = a[j - 1];
			j--;
		}
		a[j] = key;
	}
}

void selection_sort(TaggedInt *a, int n) {
	for (int i = 0; i < n; i++) {
		int min_idx = i;
		for (int j = i + 1; j < n; j++) {
			if (a[j].key < a[min_idx].key) {
				min_idx = j;
			}
		}
		if (min_idx != i) {
			swap_items(a, i, min_idx);
		}
	}
}

static void merge_ranges(TaggedInt *src, TaggedInt *dst,
	int left, int mid, int right) {
	int i = left;
	int j = mid;
	int k = left;

	while (i < mid && j < right) {
		if (src[i].key <= src[j].key) {
			dst[k++] = src[i++];
		} else {
			dst[k++] = src[j++];
		}
	}
	while (i < mid) {
		dst[k++] = src[i++];
	}
	while (j < right) {
		dst[k++] = src[j++];
	}
}

void merge_sort(TaggedInt *a, int n) {
	if (n <= 1) return;

	TaggedInt *buf = malloc((size_t)n * sizeof(TaggedInt));
	if (!buf) {
		fprintf(stderr, "Error: malloc failure\n");
		return;
	}

	TaggedInt *src = a;
	TaggedInt *dst = buf;

	for (int width = 1; width < n; width *= 2) {
		for (int left = 0; left < n; left += 2 * width) {
			int mid = left + width;
			int right = left + 2 * width;
			if (mid > n) mid = n;
			if (right > n) right = n;
			merge_ranges(src, dst, left, mid, right);
		}
		TaggedInt *tmp = src;
		src = dst;
		dst = tmp;
	}

	if (src != a) {
		for (int i = 0; i < n; i++) {
			a[i] = src[i];
		}
	}

	free(buf);
}

static int median_of_three(TaggedInt *a, int lo, int hi) {
	int mid = lo + (hi - lo) / 2;
	int x = a[lo].key;
	int y = a[mid].key;
	int z = a[hi].key;

	if ((x <= y && y <= z) || (z <= y && y <= x)) return mid;
	if ((y <= x && x <= z) || (z <= x && x <= y)) return lo;
	return hi;
}

static int partition_lomuto(TaggedInt *a, int lo, int hi) {
	int p = median_of_three(a, lo, hi);
	int pivot = a[p].key;
	swap_items(a, p, hi);

	int i = lo;
	for (int j = lo; j < hi; j++) {
		if (a[j].key < pivot) {
			swap_items(a, i, j);
			i++;
		}
	}
	swap_items(a, i, hi);
	return i;
}

static void quick_sort_range(TaggedInt *a, int lo, int hi) {
	if (lo >= hi) return;
	int p = partition_lomuto(a, lo, hi);
	quick_sort_range(a, lo, p - 1);
	quick_sort_range(a, p + 1, hi);
}

void quick_sort(TaggedInt *a, int n) {
	if (n <= 1) return;
	quick_sort_range(a, 0, n - 1);
}

static void heapify(TaggedInt *a, int n, int i) {
	while (1) {
		int largest = i;
		int left = 2 * i + 1;
		int right = 2 * i + 2;

		if (left < n && a[left].key > a[largest].key) {
			largest = left;
		}
		if (right < n && a[right].key > a[largest].key) {
			largest = right;
		}
		if (largest == i) {
			return;
		}
		swap_items(a, i, largest);
		i = largest;
	}
}

void heap_sort(TaggedInt *a, int n) {
	if (n <= 1) return;
	for (int i = n / 2 - 1; i >= 0; i--) {
		heapify(a, n, i);
	}
	for (int i = n - 1; i > 0; i--) {
		swap_items(a, 0, i);
		heapify(a, i, 0);
	}
}

void print_array(TaggedInt *a, int n) {
	for (int i = 0; i < n; i++) {
		printf("[%d, %d], ", a[i].key, a[i].id);
	}
	printf("\n");
}
