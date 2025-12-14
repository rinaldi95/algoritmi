#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


long ns_diff(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) * 1000000000L + (b.tv_nsec - a.tv_nsec);
}

int linear_search(const int *list, int len, int target) {
    for (int i = 0; i < len; i++) {
        if (list[i] == target) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    srand((unsigned)time(NULL));
    int reps = 2000; /* ridotto per linear search (O(n)) */
    printf("n,case,alg,ns_per_call,sink\n");
    for (int i = 0; i < 18; i++) {
        int n = 1 << i;
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "malloc failure\n");
            return 1;
        }
        for (int j = 0; j < n; j++) {
            arr[j] = j;
        }

        struct timespec t0, t1;
        volatile int sink = 0; /* impedisce l'ottimizzazione del loop */
        clock_gettime(CLOCK_MONOTONIC, &t0);
        for (int k = 0; k < reps; k++) {
            int target = rand() % (n + 5);
            int result = linear_search(arr, n, target);
            sink += result;
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);

        long ns = ns_diff(t0, t1);
        double per_call = (double)ns / reps;
        printf("%d,random,linear,%.2f,%d\n", n, per_call, sink);
        free(arr);
    }
    return 0;
}
