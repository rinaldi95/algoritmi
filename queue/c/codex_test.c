#include <assert.h>
#include <stdio.h>
#include "queue.h"

static void test_empty_underflow(void) {
    Queue q;
    int v;
    assert(queue_init(&q, QUEUE_DEFAULT_CAPACITY) == 0);
    assert(queue_is_empty(&q));
    assert(queue_size(&q) == 0);
    assert(queue_peek(&q, &v) == -1);
    assert(queue_dequeue(&q, &v) == -1);
    queue_destroy(&q);
}

static void test_fifo_no_resize(void) {
    Queue q;
    int v;
    assert(queue_init(&q, 4) == 0);
    for (int i = 1; i <= 4; i++) {
        assert(queue_enqueue(&q, i) == 0);
    }
    assert(queue_peek(&q, &v) == 0 && v == 1);
    for (int i = 1; i <= 4; i++) {
        assert(queue_dequeue(&q, &v) == 0);
        assert(v == i);
    }
    assert(queue_is_empty(&q));
    queue_destroy(&q);
}

static void test_wrap_no_resize(void) {
    Queue q;
    int v;
    assert(queue_init(&q, 4) == 0);
    /* Fill */
    for (int i = 1; i <= 4; i++) assert(queue_enqueue(&q, i) == 0);
    /* Pop two -> head moves */
    assert(queue_dequeue(&q, &v) == 0 && v == 1);
    assert(queue_dequeue(&q, &v) == 0 && v == 2);
    /* Push two -> wrap */
    assert(queue_enqueue(&q, 5) == 0);
    assert(queue_enqueue(&q, 6) == 0);
    /* Order should be 3,4,5,6 */
    assert(queue_dequeue(&q, &v) == 0 && v == 3);
    assert(queue_dequeue(&q, &v) == 0 && v == 4);
    assert(queue_dequeue(&q, &v) == 0 && v == 5);
    assert(queue_dequeue(&q, &v) == 0 && v == 6);
    assert(queue_is_empty(&q));
    queue_destroy(&q);
}

static void test_wrap_then_resize(void) {
    Queue q;
    int v;
    assert(queue_init(&q, 4) == 0);
    /* Fill 10,20,30,40 */
    assert(queue_enqueue(&q, 10) == 0);
    assert(queue_enqueue(&q, 20) == 0);
    assert(queue_enqueue(&q, 30) == 0);
    assert(queue_enqueue(&q, 40) == 0);
    /* Pop two -> head moves to 2 */
    assert(queue_dequeue(&q, &v) == 0 && v == 10);
    assert(queue_dequeue(&q, &v) == 0 && v == 20);
    /* Wrap writes at 0,1 */
    assert(queue_enqueue(&q, 50) == 0);
    assert(queue_enqueue(&q, 60) == 0);
    /* Now size 4, head at 2, tail at 2 (wrapped), logical order 30,40,50,60 */
    /* Next enqueue triggers resize; order must be preserved */
    assert(queue_enqueue(&q, 70) == 0);
    assert(queue_size(&q) == 5);
    assert(queue_peek(&q, &v) == 0 && v == 30);
    assert(queue_dequeue(&q, &v) == 0 && v == 30);
    assert(queue_dequeue(&q, &v) == 0 && v == 40);
    assert(queue_dequeue(&q, &v) == 0 && v == 50);
    assert(queue_dequeue(&q, &v) == 0 && v == 60);
    assert(queue_dequeue(&q, &v) == 0 && v == 70);
    assert(queue_is_empty(&q));
    queue_destroy(&q);
}

int main(void) {
    test_empty_underflow();
    test_fifo_no_resize();
    test_wrap_no_resize();
    test_wrap_then_resize();
    printf("codex_test: all tests passed\n");
    return 0;
}
