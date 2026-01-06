#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"deque.h"

#define INITIAL 4 

int deque_init(Deque *dq, int initial_capacity){
	dq->size=0;
	if (initial_capacity<1){
		fprintf(stderr,"Error: invalid initial_capacity: deque_init not performed\n");
		dq->data=NULL;
		return -2;
	}	
	dq->capacity=initial_capacity;
	dq->head=0;
	dq->tail=0;
	dq->data=malloc(dq->capacity*sizeof(int));
	if (dq->data==NULL){
		fprintf(stderr,"Error: malloc failure; deque_init not performed\n");
		return -1;
	}
	return 0;
}

void deque_destroy(Deque *dq){
	dq->size=0;
	dq->capacity=0;
	dq->head=0;
	dq->tail=0;
	if (dq->data!=NULL){
		int *temp=dq->data; //is it necessary?
		dq->data=NULL;
		free(temp);
	}
}

int deque_is_empty(const Deque *dq){
	if (dq->size==0){
		return 1;
	}
	return 0;
}
	
int deque_size(const Deque *dq){
	return dq->size;
}

int deque_push_back(Deque *dq, int value){
	if (dq->size==dq->capacity){
		int *temp=realloc(dq->data,dq->capacity*2*sizeof(int));
		if (temp==NULL){
			fprintf(stderr,"Error: realloc failure; deque_push_back not performed");
			return -1;
		}
		dq->data=temp;
		temp=NULL;
		//Should I free the old data?
		for (int i=dq->head; i<dq->capacity;i++){
			dq->data[i+dq->capacity]=dq->data[i];
		}
		if (dq->tail==dq->capacity-1){
			dq->tail+=dq->capacity;
		}
		dq->head+=dq->capacity;
		
		dq->capacity*=2;
		
	}
	if (!deque_is_empty(dq)){
		dq->tail=(dq->tail + 1) % dq->capacity;
	}
	dq->data[dq->tail]=value;
	dq->size++;
	
	return 0;
}

int deque_push_front(Deque *dq, int value){
	if (dq->size==dq->capacity){
		int *temp=realloc(dq->data,dq->capacity*2*sizeof(int));
		if (temp==NULL){
			fprintf(stderr,"Error: realloc failure; deque_push_back not performed");
			return -1;
		}
		dq->data=temp;
		temp=NULL;
		//Should I free the old data?
		for (int i=dq->head; i<dq->capacity;i++){
			dq->data[i+dq->capacity]=dq->data[i];
		}
		if (dq->tail==dq->capacity-1){
			dq->tail+=dq->capacity;
		}

		dq->head+=dq->capacity;

		dq->capacity*=2;
	}
	if (!deque_is_empty(dq)){
		dq->head=(dq->head - 1+dq->capacity) % dq->capacity;
	}
	dq->data[dq->head]=value;
	dq->size++;
	
	return 0;
}

int deque_pop_front(Deque *dq, int *out_value){
 	if (deque_is_empty(dq)){
 		return -1;
 	}
	*out_value = dq->data[dq->head];
	dq->head=(dq->head + 1) % dq->capacity;
	dq->size--;
	if (deque_is_empty(dq)){
		dq->head=0;
		dq->tail=0;
	}
	return 0;
}

int deque_pop_back(Deque *dq, int *out_value){
 	if (deque_is_empty(dq)){
 		return -1;
 	}
	*out_value = dq->data[dq->tail];
	dq->tail=(dq->tail + dq->capacity - 1) % dq->capacity;
	dq->size--;
	if (deque_is_empty(dq)){
		dq->head=0;
		dq->tail=0;
	}
	return 0;
}

int deque_front(const Deque *dq, int *out_value){
	if(deque_is_empty(dq)){
		return -1;
	}
	*out_value = dq->data[dq->head];
	return 0;
}

int deque_back(const Deque *dq, int *out_value){
	if(deque_is_empty(dq)){
		return -1;
	}
	*out_value = dq->data[dq->tail];
	return 0;
}

void deque_print(const Deque *dq){
	for (int i=0;i<dq->capacity;i++){
		printf("%d, ",dq->data[i]);
	}
	printf("\n");
}

#ifndef DEQUE_TEST
void run_tests(void){
	int out_value;
	Deque *dq=malloc(sizeof(Deque));
	if (dq==NULL){
		fprintf(stderr,"Error: malloc failure\n");
		exit(EXIT_FAILURE);
	}
//init and destroy tests
	assert(deque_init(dq,0)==-2);	
	assert(deque_init(dq,INITIAL)==0);
	deque_destroy(dq);
	deque_destroy(dq);
	assert(deque_init(dq,INITIAL)==0);
//deque is empty test when it is
	assert(deque_is_empty(dq));
	assert(deque_size(dq)==0);
	assert(deque_front(dq,&out_value)==-1);

//push test rezize not wrap around
	for (int i=0;i<10;i++){
		assert(deque_push_back(dq,i)==0);
		deque_print(dq);	
	}
	deque_destroy(dq);
	assert(deque_init(dq,INITIAL)==0);

//push tests resize on push back
	for (int i=0;i<10;i++){
		assert(deque_size(dq)==2*i);
		assert(deque_push_back(dq,i)==0);
		assert(deque_push_front(dq,-i)==0);	
		deque_print(dq);	
	}

//pop back test until is empty
	while(!deque_is_empty(dq)){
		assert(deque_pop_back(dq,&out_value)==0);
		printf("%d, ",out_value);
	}
	assert(deque_pop_back(dq,&out_value)==-1);
	printf("%d, \n",out_value);

	deque_destroy(dq);
	assert(deque_init(dq,INITIAL)==0);
	
//push tests resize on push front
	for (int i=0;i<10;i++){
		assert(deque_size(dq)==2*i);
		assert(deque_push_front(dq,-i)==0);
		assert(deque_push_back(dq,i)==0);	
		deque_print(dq);	
	}

	assert(deque_front(dq,&out_value)==0);
	assert(out_value==-9);

//pop front test until is empty
	while(!deque_is_empty(dq)){
		assert(deque_pop_front(dq,&out_value)==0);
		printf("%d, ",out_value);
	}
	assert(deque_pop_front(dq,&out_value)==-1);
	printf("%d, \n",out_value);
		
	
}


int main(void){

	run_tests();
	return 0;
}
#endif
