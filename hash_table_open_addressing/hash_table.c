#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"hash_table.h"

//---------------------------debug functions----------------------------//

static int malloc_fail_counter=-1;

void *malloc_fail(size_t size){
	if (malloc_fail_counter==0) {
		malloc_fail_counter=-1;
		return NULL;
	}
	if (malloc_fail_counter>0) malloc_fail_counter--;
	return malloc(size);
}

void *calloc_fail(int n , size_t size){
	if (malloc_fail_counter==0) {
		malloc_fail_counter=-1;
		return NULL;
	}
	if (malloc_fail_counter>0) malloc_fail_counter--;
	return calloc(n,size);
}


#define malloc malloc_fail
#define calloc calloc_fail

//------------------------------helper functions--------------------//

int hash_function(int key,int capacity){
	int result=key%capacity;
	if (result>=0) return result;
	return result+capacity;
}

int hash_table_resize(HashTable *ht,int new_size){
	HashTable new_table;
	if(hash_table_init(&new_table,new_size)!=0){
		fprintf(stderr,"Error: resize failure while creating new hash table."
		"insert not performed\n");
		return -1;
	}
	for (int i=0; i<ht->capacity;i++){
		HashEntry entry=ht->entries[i];
		if (entry.state==HASH_SLOT_OCCUPIED){
			if(hash_table_insert(&new_table,entry.key,entry.value)==-1){
				free(new_table.entries);
				fprintf(stderr,"Error: resize failure while coping."
				"insert not performed\n");
				return -1;
			}
		}
	}
	free(ht->entries);
	ht->entries=new_table.entries;
	ht->size=new_table.size;
	ht->capacity=new_table.capacity;
	return 0;
}


//-------------------------------hashtable functions------------------//

int hash_table_init(HashTable *ht, int initial_capacity){
	if (initial_capacity<1) initial_capacity=1;
	ht->capacity=initial_capacity;
	ht->size=0;
	ht->entries=malloc(ht->capacity*sizeof(HashEntry));
	if (ht->entries==NULL){
		fprintf(stderr,"Error: malloc failure. HashTable initialization not performed\n");
		return -1;
	}		
	for (int i =0;i<ht->capacity;i++){
		ht->entries[i].state=HASH_SLOT_EMPTY;
	}
	return 0;
}

void hash_table_destroy(HashTable *ht){
	if (ht->entries!=NULL){
		free(ht->entries);
		ht->entries=NULL;
	}
	ht->size=0;
	ht->capacity=0;
}

int hash_table_is_empty(const HashTable *ht){
	if (ht->size==0) return 1;
	return 0;
}

int hash_table_size(const HashTable *ht){
	return ht->size;
}

int hash_table_capacity(const HashTable *ht){
	return ht->capacity;
}

int hash_table_insert(HashTable *ht, int key, int value){
	if(ht->size>=ht->capacity*HASH_TABLE_DEFAULT_MAX_LOAD){
		if (hash_table_resize(ht,ht->capacity*2)!=0) return -1;	
	}
	int idx=hash_function(key, ht->capacity);
	HashEntry *first_deleted=NULL;

	for(int i =idx; i< idx+ht->capacity; i++){
		idx=i%ht->capacity;
		if (ht->entries[idx].key==key){
			if(ht->entries[idx].state==HASH_SLOT_OCCUPIED){
				ht->entries[idx].value=value;
				return 1;
			}
		}
		if (ht->entries[idx].state==HASH_SLOT_EMPTY){
			if (first_deleted==NULL){
				ht->entries[idx].key=key;
				ht->entries[idx].value=value;
				ht->entries[idx].state=HASH_SLOT_OCCUPIED;
			}
			else {
				first_deleted->key=key;
				first_deleted->value=value;
				first_deleted->state=HASH_SLOT_OCCUPIED;
			}
			ht->size++;
			return 0;
		}	
		else if  ((first_deleted==NULL)
		&&(ht->entries[idx].state==HASH_SLOT_DELETED)){
			first_deleted=&ht->entries[idx];
		}
	}
	if (first_deleted!=NULL){
		first_deleted->key=key;
		first_deleted->value=value;
		first_deleted->state=HASH_SLOT_OCCUPIED;
		ht->size++;
		return 0;
	}
		
	fprintf(stderr,"Error: hash table is full. Insert not performed");
	return -1;
}

int hash_table_find(const HashTable *ht, int key,int *out_value){
	int idx=hash_function(key, ht->capacity);
	for (int i=idx;i<idx+ht->capacity;i++){
		idx=i%ht->capacity;
		if (ht->entries[idx].state==HASH_SLOT_EMPTY){
			return 0;
		}
		if((ht->entries[idx].key==key)
		&&(ht->entries[idx].state==HASH_SLOT_OCCUPIED)){
			*out_value=ht->entries[idx].value;
			return 1;
		}
	}
	return 0;
}

int hash_table_remove(HashTable *ht, int key){
	int idx=hash_function(key, ht->capacity);
	for (int i=idx;i<idx+ht->capacity;i++){
		idx=i%ht->capacity;
		if (ht->entries[idx].state==HASH_SLOT_EMPTY){
			return 0;
		}
		if((ht->entries[idx].key==key)
		&&(ht->entries[idx].state==HASH_SLOT_OCCUPIED)){
			ht->entries[idx].state=HASH_SLOT_DELETED;
			ht->size--;
			return 1;
		}
	}
	return 0;
}
	
			

void tests(HashTable *ht){
	int out_value;
	//test init failure
	malloc_fail_counter=0;
	//test init succes and destroy
	assert(hash_table_init(ht,HASH_TABLE_DEFAULT_CAPACITY)==-1);
	assert(hash_table_init(ht,HASH_TABLE_DEFAULT_CAPACITY)==0);
	hash_table_destroy(ht);
	hash_table_destroy(ht);
	assert(hash_table_init(ht,HASH_TABLE_DEFAULT_CAPACITY)==0);
	assert(hash_table_is_empty(ht));
	assert(hash_table_capacity(ht)==HASH_TABLE_DEFAULT_CAPACITY);
	assert(hash_table_size(ht)==0);
	//test find on emptu hashtable
	assert(hash_table_find(ht,0,&out_value)==0);
	//test insert on empty hashtable
	assert(hash_table_insert(ht,1,10)==0);
	//test insert in non empty hashtable
	assert(hash_table_insert(ht,2,20)==0);
	//test fine
	assert(hash_table_find(ht,1,&out_value)==1);
	assert(out_value==10);
	//test insert in already present key
	assert(hash_table_insert(ht,1,10)==1);
	assert(hash_table_size(ht)==2);
	//test key with a different key but same hash
	assert(hash_table_insert(ht,9,90)==0);
	assert(hash_table_size(ht)==3);
	//test find and remove when probing is necessary
	assert(hash_table_remove(ht,1)==1);
	assert(hash_table_find(ht,9,&out_value)==1);
	assert(out_value==90);
	assert(hash_table_find(ht,1,&out_value)==0);
	assert(out_value==90);
	out_value=0;
	assert(hash_table_size(ht)==2);
	assert(hash_table_remove(ht,1)==0);
	assert(hash_table_remove(ht,9)==1);
	assert(hash_table_remove(ht,9)==0);
	assert(hash_table_size(ht)==1);
	assert(hash_table_find(ht,9,&out_value)==0);
	assert(out_value==0);
	hash_table_destroy(ht);
	//test resize failure
	hash_table_init(ht,HASH_TABLE_DEFAULT_CAPACITY);
	for (int i=0;i<6;i++){
		assert(hash_table_insert(ht,i,i*100)==0);
		assert(hash_table_size(ht)==i+1);
	}
	malloc_fail_counter=0;
	//test find
	assert(hash_table_insert(ht,5,0)==-1);
	assert(hash_table_find(ht,5,&out_value)==1);
	assert(out_value==500);
	assert(hash_table_find(ht,6,&out_value)==0);
	assert(out_value==500);
	//test remove until is empty
	for (int i=0;i<6;i++){
		assert(hash_table_size(ht)==6-i);
		assert(hash_table_remove(ht,i)==1);
	}
	assert(hash_table_is_empty(ht));
	//test resize 
	for (int i=0;i<6;i++){
		assert(hash_table_insert(ht,i,i*100)==0);
		assert(hash_table_size(ht)==i+1);
	}
	assert(hash_table_insert(ht,5,0)==1);
	for (int i=6;i<20;i++){
		assert(hash_table_insert(ht,i,i*100)==0);
		assert(hash_table_size(ht)==i+1);
	}
	assert(hash_table_capacity(ht)==32);
	assert(hash_table_size(ht)==20);

	printf("All tests passed\n");
}



int main(void){
	HashTable ht;
	tests(&ht);
	return 0;
}
