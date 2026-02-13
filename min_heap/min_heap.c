#include<stdio.h>
#include<stdlib.h>
#include<assert.h>


#include"min_heap.h"

/*================================helpers==================*/

/* - parent(i), left(i), right(i) (calcoli indici)
  - swap(i, j) (scambio in array)
  - sift_up(i) (risalita dopo insert)
  - sift_down(i) (discesa dopo extract/heapify)
  - ensure_capacity() / resize() (realloc quando pieno)

  Helper utili ma opzionali:

  - heapify() (O(n) da array) → spesso esposto come min_heap_build
  - is_valid() (verifica invarianti, utile per test/debug)
  */


static int parent(int i){
	return ((int)((i-1)/2));
}

static int left(int i){
	return(2*i+1);
}
static int right(int i){
	return (2*i+2);
}

static int heap_is_valid(MinHeap *heap){
	if (heap->size>heap->capacity) return 0;
	for (int i=0;i<heap->size;i++){
		if (heap->data[i]<heap->data[parent(i)]) return 0;
		if (left(i)<heap->size && heap->data[i]>heap->data[left(i)]) return 0;
		if (right(i)<heap->size && heap->data[i]>heap->data[right(i)]) return 0;			
	}
	return 1;
}

static void swap(int *data, int i,int j){
	int temp=data[i];
	data[i]=data[j];
	data[j]=temp;
}

static void sift_up(int *data,int i){
	while(data[i]<data[parent(i)]){
		swap(data,i,parent(i));
		i=parent(i);
	}
}

static void sift_down(int *data, int size, int i){
	int l=left(i);
	int r=right(i);
	while (l<size){
		if (r==size||data[l]<data[r]){
			if(data[i]>data[l]) {
				swap(data,i,l);
				i=l;
				l=left(i);
				r=right(i);
			}
			else l=size;
		}
		else{
			if(data[i]>data[r]) {
				swap(data,i,r);
				i=r;
				l=left(i);
				r=right(i);
			}
			else l=size;
		}
	}
}
		

static int resize(MinHeap *heap,int  new_capacity){
	if (new_capacity<heap->size){
		fprintf(stderr,"Error: new_capacity<size. resize not performed");
		return 1;
	}
	int *new_data=heap->data;
	new_data=realloc(heap->data,new_capacity *sizeof(int));
	if (new_data==NULL){
		fprintf(stderr,"Error: realloc failure. resize not performed");
		return -1;
	}
	heap->data=new_data;
	heap->capacity=new_capacity;
	return 0;
}
	

static void heap_print(const MinHeap *heap){
	for (int i=0; i<heap->size; i++){
		printf("%d, ", heap->data[i]);
	}
	printf("\n");
}

static void heapify(int *data, int n){
	if (n>0){
		int i=parent(n-1);
		while(i>=0) sift_down(data,n,i--);
	}
} 



int min_heap_init(MinHeap *heap, int initial_capacity){
	if (initial_capacity<1) initial_capacity=1;
	heap->size=0;
	heap->data=malloc(initial_capacity * sizeof(int));
	if( heap->data==NULL){
		fprintf(stderr,"Error: malloc failure. min_heap_init not performed\n");
		min_heap_destroy(heap);
		return -1;
	}
	heap->capacity=initial_capacity;
	return 0;
}

void min_heap_destroy(MinHeap *heap){
	if (heap->data!=NULL) {
		free(heap->data);
		heap->data=NULL;
	}
	heap->size=0;
	heap->capacity=0;
}

int min_heap_is_empty(const MinHeap *heap){
	if (heap->size==0) return 1;
	return 0;
} 

int min_heap_size(const MinHeap *heap){
	return heap->size;
}

int min_heap_push(MinHeap *heap,int value){
	if (heap->size==heap->capacity) {	
		int esit=resize(heap,2*heap->capacity);
		if (esit!=0) return esit;
	}
	int i=heap->size;
	heap->data[i]=value;
	sift_up(heap->data,i);
	heap->size++;
	return 0;
}
	
int min_heap_peek(const MinHeap *heap, int *out_value){
	if (min_heap_is_empty(heap)) return 1;
	*out_value=heap->data[0];
	return 0;
}

int min_heap_extract_min(MinHeap *heap, int *out_value){
	if (min_heap_is_empty(heap)) return 1;
	*out_value=heap->data[0];
	heap->size--;
	swap(heap->data,0,heap->size);
	sift_down(heap->data,heap->size,0);
	return 0;
}

int min_heap_build(MinHeap *heap, const int *values, int n){
	if (n<0){
		fprintf(stderr,"Error: lenght of array is <0. heapify not performed\n");
		return 1;
	}
	if (values==NULL){
		fprintf(stderr,"Error: value is invalid. heapify not performed");
		return 1;
	}
	while (heap->capacity<n) {
		if (resize(heap,2*heap->capacity)!=0) return -1;
	}
	heap->size=n;
	for (int i=0;i<n;i++){
		heap->data[i]=values[i];
	}
	heapify(heap->data,n);
	return 0;
}

void run_tests(MinHeap *heap){
	assert(min_heap_init(heap,MIN_HEAP_DEFAULT_CAPACITY)==0);
	assert(min_heap_is_empty(heap));
	for (int i=41;i<200;i++){
		int new=((i*i*i)<<4)%83;
		printf("insert value %d\n",new);
		assert(min_heap_push(heap,new)==0);
		heap_print(heap);
		assert(heap_is_valid(heap));
	}
	int value;
	while(!min_heap_is_empty(heap)){
		min_heap_peek(heap,&value);
		printf("deleting value %d\n", value);
		assert(min_heap_extract_min(heap, &value)==0);
		heap_print(heap);
		assert(heap_is_valid(heap));
	}
	printf("all tests passed\n");
}

int main(void){
	MinHeap heap;
	run_tests(&heap);
	min_heap_destroy(&heap);
	
	return 0;
}
