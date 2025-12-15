#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

#define QUEUE_DEFAULT_CAPACITY 4

typedef struct{
	int *data;
	int size;
	int capacity;
	int tail;
	int head;
} Queue;

int queue_init(Queue* q, int initial_capacity);
void queue_destroy(Queue* q);
int queue_is_empty(const Queue* q);
int queue_size(const Queue* q);
int queue_enqueue(Queue* q, int value);
int queue_peek(const Queue* q, int* value);
int queue_dequeue(Queue* q, int* value);

#endif
