#ifndef STACK_H
#define STACK_H

#include <stddef.h>

#define STACK_DEFAULT_CAPACITY 4

typedef struct {
    int *data;
    int size;
    int capacity;
} Stack;

int stack_init(Stack *s, int initial_capacity);
void stack_destroy(Stack *s);
int stack_is_empty(const Stack *s);
int stack_size(const Stack *s);
int stack_push(Stack *s, int value);
int stack_top(const Stack *s, int *out_value);
int stack_pop(Stack *s, int *out_value);

#endif
