#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"doubly_linked_list.h"

int dlist_init(DList* l){
	l->head=NULL;
	l->tail=NULL;
	return 0;
}

void dlist_destroy(DList*l){
	if (!dlist_is_empty(l)){
		while(l->head->next!=NULL){
			//printf("%d\n",l->head->value);
			l->head=l->head->next;
			free(l->head->prev);
		}
		free(l->head);
	}
	dlist_init(l);
}

int dlist_is_empty(const DList* l){
	if(l->head==NULL || l->tail==NULL){
		assert(l->head==NULL && l->tail==NULL);
		return 1;
	}
	return 0;
}

int dlist_push_front(DList* l, int value){
	if (dlist_is_empty(l)){
		l->head=malloc(sizeof(DNode));
		if (l->head==NULL){
			fprintf(stderr,"malloc failure\n");
			return 1;
		}
		l->tail=l->head;
		l->head->next=NULL;
		l->tail->prev=NULL;
	}
	else{
		l->head->prev=malloc(sizeof(DNode));
		if (l->head->prev==NULL){
			fprintf(stderr,"malloc failure\n");
			return 1;
		}
		l->head->prev->next=l->head;
		l->head=l->head->prev;
	}
	l->head->prev=NULL;
	l->head->value=value;
	return 0;
}		

int dlist_push_back(DList* l, int value){
	if (dlist_is_empty(l)){
		l->head=malloc(sizeof(DNode));
		if (l->head==NULL){
			fprintf(stderr,"malloc failure\n");
			return 1;
		}
		l->tail=l->head;
		l->tail->prev=NULL;
		l->head->next=NULL;
	}
	else{
		l->tail->next=malloc(sizeof(DNode));
		if (l->tail->next==NULL){
			fprintf(stderr,"malloc failure\n");
			return 1;
		}
		l->tail->next->prev=l->tail;
		l->tail=l->tail->next;
	}
	l->tail->next=NULL;
	l->tail->value=value;
	return 0;
}		

DNode* dlist_find(const DList* l, int value){
	if (dlist_is_empty(l)) return NULL;
	DNode* node=l->head;
	while(node->value!=value){
		node=node->next;
		if (node==NULL){
			return NULL;	// How much time could I save by storing the length of the list?
		}
	}
	return node;
}

// This function doesn't check that the node belongs to the list; the caller must guarantee it.
int dlist_insert_after(DList* l, DNode* node, int value){
	if (node==NULL){
		fprintf(stderr,"dlist_insert_after failed: argument is NULL; insert not performed.\n");
		return 1;
	}
	
	if (node==l->tail){
		return dlist_push_back(l,value);
	}
	DNode* new=malloc(sizeof(DNode));
	if (new==NULL){
		fprintf(stderr,"error: malloc failure, dlist_insert_after not performed\n");
		return 1;
	}
	new->next=node->next;
	node->next->prev=new;
	node->next=new;
	new->prev=node;
	new->value=value;
	
	return 0;
}

int dlist_delete_value(DList* l, int value){
	DNode* node=dlist_find(l,value);
	if (node==NULL){
		return 1;
	}	
	if (node==l->tail){
		if(node==l->head){
			dlist_init(l);
			free(node);
			return 0;
		}
		l->tail=l->tail->prev;
		l->tail->next=NULL;
		free(node);
	}
	else if (node==l->head){
		l->head=l->head->next;
		l->head->prev=NULL;
		free(node);
	}
	else{
		node->prev->next= node->next;
		node->next->prev=node->prev;
		free(node);
	}
	return 0;
}

void dlist_print_forward(const DList* l){
	if (!dlist_is_empty(l)){
		DNode* node=l->head;
		while(node->next!=NULL){
			printf("%d, ",node->value);
			node=node->next;
		}
		printf("%d\n",node->value);
	}
	else fprintf(stderr,"list is empty\n");
}

void dlist_print_backward(const DList* l){
	if (!dlist_is_empty(l)){
		DNode* node=l->tail;
		while(node->prev!=NULL){
			printf("%d, ",node->value);
			node=node->prev;
		}
		printf("%d\n",node->value);
	}
	else fprintf(stderr,"list is empty\n");
}
	
		




void run_tests(){
	
	int headvalue=0;
	int tailvalue=0;
	DList* l;
	l= malloc(sizeof(DList));
	if(l==NULL){
		fprintf(stderr,"Malloc failure\n");
	}
	for (int i=0;i<10;i++){
	
		dlist_init(l);
		for (int j=0;j<i;j++){
			assert(dlist_push_front(l,headvalue)==0);
		}
		dlist_destroy(l);//test with j elements
	}
		
	assert(dlist_init(l)==0);
	// test list is empty when it is empty
	assert(dlist_is_empty(l));
	// test find value when list is empty
	assert(dlist_find(l,0)==NULL);
	assert(dlist_push_front(l,headvalue)==0);
	headvalue++;
	assert(dlist_push_back(l,tailvalue)==0);
	tailvalue--;
	// test list is empty when not empty
	assert(dlist_is_empty(l)==0);
	// test push
	for (int i=0;i<10;i++){
		assert(dlist_push_front(l,i)==0);
		assert(dlist_push_back(l,-i)==0);
	}
	// test find
	assert(dlist_find(l,30)==NULL);
	assert(dlist_find(l,9)==l->head);
	assert(dlist_find(l,-9)==l->tail);
	// test inserts
	DNode* node= dlist_find(l,4);
	assert(node!=NULL);
	assert(dlist_insert_after(l,node,30)==0);
	assert(dlist_insert_after(l,l->head,60)==0);
	assert(dlist_insert_after(l,l->tail,90)==0);
	assert(dlist_find(l,30)==node->next);
	assert(dlist_find(l,60)==l->head->next);
	assert(dlist_find(l,90)==l->tail);	
	//test delete not empty
	assert(dlist_delete_value(l,30)==0);
	assert(dlist_delete_value(l,9)==0);
	assert(dlist_delete_value(l,90)==0);
	assert(dlist_delete_value(l,5000)==1);
	assert(l->head->value==60);
	assert(l->tail->value==-9);
	assert(dlist_find(l,30)==NULL);
	
	//test print
	dlist_print_forward(l);
	dlist_print_backward(l);
	//test delete until empty
	while(dlist_is_empty(l)==0){
		printf("%d\n",l->head->value);
		dlist_delete_value(l,l->head->value);
		dlist_print_forward(l);
	}

	dlist_print_forward(l);
	dlist_destroy(l);
	free(l);
}

int main(void){

	run_tests();
	return 0;
}
