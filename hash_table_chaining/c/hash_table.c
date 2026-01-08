#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include"hash_table.h"



//---------------------test functions---------------------------//

static int malloc_fail_after = -1;

void *test_malloc(size_t n) {
	if (malloc_fail_after == 0) return NULL;
	if (malloc_fail_after > 0) malloc_fail_after--;
	return malloc(n);
}

#define malloc test_malloc
//#undef malloc

//--------------------------helper functions----------------------//



void hash_node_create(HashNode *hn, int key, int value){
	hn->key=key;
	hn->value=value;
	hn->next=NULL;
} 

int hash_function(int key, int capacity){
	int out=key%capacity;
	if (out<0) out+=capacity;
	return out;
}


//--------------------------singly linked list of hashnodes------------//



void bucket_destroy(HashNode *hn){
	while(hn!=NULL){
		HashNode *tmp=hn;
		hn=hn->next;
		free(tmp);
	}
}


//--------------------------HashTable----------------------------//

int hash_table_init(HashTable *ht, int initial_capacity){
	if (initial_capacity<1) initial_capacity=1;
	ht->capacity=initial_capacity;
	ht->size=0;
	ht->buckets=calloc(ht->capacity,sizeof(HashNode*));
	if (ht->buckets==NULL){
		fprintf(stderr,"Error: calloc failure. HashTable not initialized\n");
		return -1;
	}
	return 0;
}

void hash_table_destroy(HashTable *ht){
	if (ht->buckets!=NULL){
		for (int i=0; i<ht->capacity; i++){
			bucket_destroy(ht->buckets[i]);
		}
		free(ht->buckets);
		ht->buckets=NULL;
	}
	ht->capacity=0;
	ht->size=0;
}

int hash_table_is_empty(const HashTable *ht){
	if (ht->size==0) return 1;
	else return 0;
}

int hash_table_size(const HashTable *ht){
	return ht->size;
}

int hash_table_capacity(const HashTable *ht){
	return ht->capacity;
}

int hash_table_insert(HashTable *ht, int key, int value){
	if (ht->size>ht->capacity*HASH_TABLE_DEFAULT_MAX_LOAD){
		HashTable *htnew=malloc(sizeof(HashTable));
		if (htnew==NULL){
			fprintf(stderr,"Error: Malloc failure. resize not performed");
			return -1;
		}
		if(hash_table_init(htnew,2*ht->capacity)==-1){
			free(htnew);
			htnew=NULL;
			return -1;
		}
		for (int i=0;i<ht->capacity;i++){
			HashNode *hn=ht->buckets[i];
			while(hn!=NULL){
				if (hash_table_insert(htnew,hn->key,hn->value)==-1){
					hash_table_destroy(htnew);
					free(htnew);
					htnew=NULL;
					return -1;
				}
				hn=hn->next;
			}
		}
		hash_table_destroy(ht);
		ht->capacity=htnew->capacity;
		ht->size=htnew->size;
		ht->buckets=htnew->buckets;
		free(htnew);
	}
	int idx=hash_function(key,ht->capacity);
	HashNode *hn=ht->buckets[idx];
	HashNode *prev=NULL;
	while(hn!=NULL){
		if (hn->key==key) {
			hn->value=value;
			return 1;
		}
		prev=hn;
		hn=hn->next;
	}
	hn=malloc(sizeof(HashNode));
	if (hn==NULL){
		fprintf(stderr,"Error: malloc failure. New bucket not created\n");
		return -1;
	}
	if (prev!=NULL) prev->next=hn;
	hash_node_create(hn,key,value);
	if (ht->buckets[idx]==NULL) ht->buckets[idx]=hn;
	ht->size++;
	return 0;
}

int hash_table_find(const HashTable *ht, int key, int *out_value){
	int head=hash_function(key, ht->capacity);
	HashNode *hn= ht->buckets[head];
	while(hn!=NULL){
		if (key==hn->key){
			*out_value=hn->value;
			return 1;
		}
		hn=hn->next;
	}
	return 0;
}

int hash_table_remove(HashTable *ht, int key){
	int head=hash_function(key, ht->capacity);
	HashNode *hn= ht->buckets[head];
	if ((hn!=NULL)&&(key==hn->key)){
		ht->buckets[head]=hn->next;
		hn->next=NULL;
		hn->value=0;
		hn->key=0;
		free(hn);
		ht->size--;
		return 1;
	}
	HashNode *tmp=NULL;
	while(hn!=NULL){
		if (key==hn->key){
			tmp->next=hn->next;
			hn->next=NULL;
			hn->value=0;
			hn->key=0;
			free(hn);
			ht->size--;
			return 1;
		}
		tmp=hn;
		hn=hn->next;
	}
	return 0;
}


void tests(HashTable *ht){
	int out_value;
	//test init
	assert(hash_table_init(ht, HASH_TABLE_DEFAULT_CAPACITY)==0);
	assert(hash_table_size(ht)==0);
	//test destroy on empty hashtable
	hash_table_destroy(ht);
	assert(ht->buckets==NULL);
	assert(hash_table_size(ht)==0);
	assert(hash_table_capacity(ht)==0);
	assert(hash_table_is_empty(ht));
	
	assert(hash_table_init(ht, HASH_TABLE_DEFAULT_CAPACITY)==0);
	//test functions on empty hashtable
	assert(hash_table_find(ht,0,&out_value)==0);
	assert(hash_table_remove(ht,40)==0);
	assert(hash_table_is_empty(ht));
	assert(hash_table_insert(ht,1,100)==0);
	//test insert on empty hashtable
	assert(hash_table_is_empty(ht)==0);
	assert(hash_table_insert(ht,2,200)==0);
	//test insert in a not empty hashtable but on a empty bucket 
	assert(hash_table_insert(ht,1,2000)==1);
	//test insert hashtable in empty bucket + resize
	for (int i=3;i<9;i++){
		assert(hash_table_insert(ht,i,100*i)==0);
		assert(hash_table_size(ht)==i);
	}
	//resize check
	assert(hash_table_capacity(ht)==16);
	//test find and if insert put the value correctly
	assert(hash_table_find(ht,0,&out_value)==0);
	assert(hash_table_find(ht,1,&out_value)==1);
	assert(out_value==2000);
	//test insert on not empty hashtable and not empty bucket + resize
	for (int i=16;i<100;i+=8){
		assert(hash_table_insert(ht,i,1000*i)==0);
		assert(hash_table_size(ht)==9+i/8-2);
	}
	assert(hash_table_size(ht)==19);
	//resize check
	assert(hash_table_capacity(ht)==32);
	//test find + remove in the middle of a bucket
	assert(hash_table_find(ht,40,&out_value)==1);
	assert(out_value==40000);
	assert(hash_table_remove(ht,40)==1);
	assert(hash_table_size(ht)==18);
	//check if the previus and the next node are correctly connected
	assert(hash_table_find(ht,48,&out_value)==1);
	assert(hash_table_find(ht,32,&out_value)==1);
	//test find + remove at the end of a bucket
	assert(hash_table_find(ht,96,&out_value)==1);
	assert(out_value==96000);
	assert(hash_table_remove(ht,96)==1);
	assert(hash_table_find(ht,96,&out_value)==0);
	assert(out_value==96000);
	//test find + remove on the first element 
	//of a bucket with many elements
	assert(hash_table_find(ht,8,&out_value)==1);
	assert(out_value==800);
	assert(hash_table_remove(ht,8)==1);
	assert(hash_table_find(ht,8,&out_value)==0);
	assert(out_value==800);
	//check if the next value is the new head
	assert(hash_table_find(ht,72,&out_value)==1);
	assert(ht->buckets[8]->key==72);
	assert(out_value==72000);
	//test find + remove in a bucket with 1 item
	assert(hash_table_find(ht,1,&out_value)==1);
	assert(out_value==2000);
	assert(hash_table_remove(ht,1)==1);
	assert(hash_table_size(ht)==15);
	//test double remove
	assert(hash_table_remove(ht,1)==0);
	assert(hash_table_size(ht)==15);
	assert(hash_table_find(ht,1,&out_value)==0);
	assert(out_value==2000);

	//malloc failure tests
	malloc_fail_after=0;
	assert(hash_table_insert(ht,30,50)==-1);
	malloc_fail_after=-1;
	

	//test destroy on not empty hashtable
	hash_table_destroy(ht);
	assert(ht->buckets==NULL);
	assert(hash_table_size(ht)==0);
	assert(hash_table_capacity(ht)==0);
	assert(hash_table_is_empty(ht));
}

int main(void){
	HashTable ht;
	tests(&ht);
	return 0;
}



