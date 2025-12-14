#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "stack.h"

void stack_destroy(Stack* s){
	s->capacity=0;
	free(s->data);
	s->data=NULL;
	s->size=0;
}

int stack_is_empty(const Stack* s){
	return s->size==0;
}

int stack_size(const Stack* s){
	return s->size;
}

int stack_init(Stack* s, int initial_capacity){
	if (initial_capacity<1) initial_capacity=1; 
	s->capacity=initial_capacity;
	s->data=malloc(s->capacity*sizeof(int));
	if (s->data==NULL){
		fprintf(stderr,"malloc failure");
		return -1;
	}
	s->size=0;
	return 0;
}

int stack_push(Stack* s, int value){
	if (s->size==s->capacity){
		int new_capacity=2*s->capacity;
		int* new_data=realloc(s->data,new_capacity*sizeof(int));
		if(new_data==NULL){
			fprintf(stderr,"realloc failure");
			return -1;
		}
		s->capacity=new_capacity;
		s->data=new_data;
	}
	s->data[s->size]=value;
	s->size++;
	return 0;
}

int stack_top(const Stack* s, int* out_value){
	if (stack_is_empty(s)){
		fprintf(stderr,"stack_top error: stack is empty");
		return -1;
	}
	*out_value=s->data[s->size-1];
	return 0;
}

int stack_pop(Stack* s, int* out_value){
	if (stack_is_empty(s)){
		fprintf(stderr,"stack_pop error: stack is empty");
		return -1;
	}
	s->size=s->size-1;
	*out_value=s->data[s->size];
	s->data[s->size]=0;
	return 0;
}

static void run_tests(void){
	Stack s;
	int result=stack_init(&s,STACK_DEFAULT_CAPACITY);
	assert(result == 0);
	assert(stack_is_empty(&s));
	assert(stack_size(&s)==0);

	int value;

	/* Underflow tests */
	assert(stack_top(&s,&value)==-1);
	assert(stack_pop(&s,&value)==-1);
	assert(stack_is_empty(&s));
	assert(stack_size(&s)==0);

	/* Push one, top, pop */
	assert(stack_push(&s,42)==0);
	assert(stack_top(&s,&value)==0 && value==42);
	assert(stack_pop(&s,&value)==0 && value==42);
	assert(stack_is_empty(&s));

	/* Alternating push/pop */
	assert(stack_push(&s,1)==0);
	assert(stack_push(&s,2)==0);
	assert(stack_pop(&s,&value)==0 && value==2);
	assert(stack_top(&s,&value)==0 && value==1);
	assert(stack_pop(&s,&value)==0 && value==1);
	assert(stack_is_empty(&s));

	/* Resize test: push 5 elements with initial cap 4 */
	int vals[] = {10,20,30,40,50};
	for (int i=0; i<5; i++){
		assert(stack_push(&s, vals[i])==0);
		assert(stack_size(&s)==i+1);
	}
	assert(s.capacity==8); /* cap 4 -> push 5 -> resize to 8 */
	for (int i=0; i<5; i++){
		assert(stack_pop(&s, &value)==0);
		assert(value==vals[4-i]);
	}
	assert(stack_is_empty(&s));
	assert(stack_size(&s)==0);

	stack_destroy(&s);
	assert(s.data==NULL);
	assert(s.capacity==0);
	assert(s.size==0);
}

int main(void){
	run_tests();
	printf("All tests passed\n");
	return 0;
}
