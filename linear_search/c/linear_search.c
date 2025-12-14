#include <stdio.h>

int linear_search(const int *list, int len, int target) {
    for (int i = 0; i < len; i++) {
        if (list[i] == target) {
            return i;
        }
    }
    return -1;
}

struct test{
    int arr[8];
    int len;
    int target;
    int expected;
};

void run_tests(void) {
    struct test cases[] = {
        {{}, 0, 5, -1},
        {{5}, 1, 5, 0},
        {{5}, 1, 7, -1},
        {{1, 2, 3}, 3, 1, 0},
        {{1, 2, 3}, 3, 3, 2},
        {{1, 2, 3}, 3, 4, -1},
        {{4, 4, 4}, 3, 4, 0},
        {{2, 9, 2, 9}, 4, 9, 1},
        {{2, 9, 2, 9}, 4, 2, 0},
    };
    int total = (int)(sizeof(cases) / sizeof(cases[0]));
    int pass = 0;
    for (int i = 0; i < total; i++) {
        int got = linear_search(cases[i].arr, cases[i].len, cases[i].target);
        if (got == cases[i].expected) {
            pass++;
        } else {
            printf("Fail #%d: got %d, expected %d\n", i, got, cases[i].expected);
        }
    }
    printf("Passed %d/%d\n", pass, total);
}

int main(void) {
    run_tests();
    return 0;
}
