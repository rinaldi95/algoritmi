#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

void run_tests(Queue* q){
	int value=9;
	int peek;
	int dequeue;
	int size;

//test init
	assert(queue_init(q,QUEUE_DEFAULT_CAPACITY)==0);
	assert(queue_is_empty(q));	
	assert(queue_size(q)==0);
	assert(queue_peek(q,&peek)==-1);
	assert(queue_dequeue(q,&dequeue)==-1);

	
//test enqueue
	for (int i=0;i<4;i++){
		assert(queue_enqueue(q,i+value)==0);
		assert(!queue_is_empty(q));	
		assert(queue_size(q)==i+1);
	}
//test peek
	assert(queue_peek(q,&peek)==0);
	assert(peek==value);

//test resize no wrap around
	assert(queue_enqueue(q,value+4)==0);
	assert(!queue_is_empty(q));	
	assert(queue_size(q)==5);

	
	assert(queue_peek(q,&peek)==0);
	assert(peek==value);
//test dequeue
	size=queue_size(q);

	for (int i=0;i<2;i++){
		assert(queue_dequeue(q,&dequeue)==0);
		assert(!queue_is_empty(q));	
		assert(queue_size(q)==(size-i-1));
	}
//test resize with wrap around
	for (int i=5;i<20;i++){
		assert(queue_enqueue(q,i+value)==0);
	}
	int i=0;
	while (!queue_is_empty(q)){
		assert(queue_dequeue(q,&dequeue)==0);	
		assert(dequeue==11+i);
		i++;
	}

	assert(queue_is_empty(q));	
	assert(queue_size(q)==0);
	assert(queue_peek(q,&peek)==-1);
	assert(queue_dequeue(q,&dequeue)==-1);
}


int queue_init(Queue* q, int initial_capacity){
	q->size=0;
	if (initial_capacity<1) initial_capacity=QUEUE_DEFAULT_CAPACITY;
	q->capacity=initial_capacity;
	q->head=0;
	q->tail=0;
	q->data=malloc(initial_capacity*sizeof(int));
	if (q->data==NULL){
		fprintf(stderr,"malloc error\n");
		return -1;
	}
	return 0;
}

void queue_destroy(Queue* q){
	q->size=0;
	q->capacity=0;
	q->head=0;
	q->tail=0;
	free(q->data);
	q->data=NULL;
}


int queue_is_empty(const Queue* q){
	return (q->size==0);
}

int queue_size(const Queue* q){
	return q->size;
}

int queue_enqueue(Queue* q, int value){
	if (q->size==q->capacity){
		int new_capacity=q->capacity*2;
		int *new_data=realloc(q->data,new_capacity*sizeof(int));
		if (new_data==NULL){
			fprintf(stderr,"realloc failure: enqueue not performed\n");
			return -1;
		}

		q->data=new_data;
		for (int i=q->head;i<q->capacity;i++){
			q->data[i+q->capacity]=q->data[i];
			q->data[i]=0;
		}
		q->head=q->head+q->capacity;
		q->capacity=new_capacity;
	}
	q->data[q->tail]=value;
	q->tail=(q->tail+1)%q->capacity;
	q->size++;
	return 0;
}

int queue_peek(const Queue* q, int* value){
	if (queue_is_empty(q)){
		fprintf(stderr,"peek failure. queue is empty\n");
		return -1;
	}
	*value = q->data[q->head];
	return 0;
}
	
int queue_dequeue(Queue* q, int* value){
	if (queue_is_empty(q)){
		fprintf(stderr,"dequeue failure. queue is empty\n");
		return -1;
	}
	*value = q->data[q->head];
	q->data[q->head]=0;
	q->head=(q->head+1)%q->capacity;
	q->size--;
	return 0;
}	

int main(void){
	Queue q;
	run_tests(&q);
	queue_destroy(&q);	
	return 0;
}
