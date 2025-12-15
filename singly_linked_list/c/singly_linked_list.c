#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include"singly_linked_list.h"

void run_tests(List* l){
	int headvalue=0;
	int tailvalue=0;
	Node* n;
//init test
	assert(list_init(l)==0);
	assert(list_is_empty(l));
	list_find(l,0);
//push front test
	while(headvalue>-3){
		assert(list_push_front(l,headvalue)==0);
		headvalue--;
		assert(list_is_empty(l)==0);
	}
//push back test
	while(tailvalue<3){
		tailvalue++;
		assert(list_push_back(l,tailvalue)==0);
	}
// find test
	for (int i=headvalue; i<=tailvalue; i++){
		 n=list_find(l,i);
		 if (n!=NULL){
			 printf("i=%d node=%p value=%d next=%p\n",i, (void*)n,
			 	n->value, (void*)n->next);
		 }
		 else printf("valore %d non trovato\n",i);
	}
//delete 1not found 2head 3tail 4middle
	assert(list_delete_value(l,headvalue-2)==0);
	assert(list_delete_value(l,headvalue+1));
	assert(list_delete_value(l,tailvalue));
	assert(list_delete_value(l,headvalue+3));
	
	for (int i=headvalue; i<=tailvalue; i++){
		 n=list_find(l,i);
		 if (n!=NULL){
			 printf("i=%d node=%p value=%d next=%p\n",i, (void*)n,
			 	n->value, (void*)n->next);
		 }
		 else printf("valore %d non trovato\n",i);
	}
//list print
	list_print(l);
//azzeramento lista
	for (int i =-10;i<10; i++){
		list_delete_value(l,i);
	}
	assert(list_is_empty(l));
	//push front test REVERSE
	
	while(tailvalue<6){
		tailvalue++;
		assert(list_push_back(l,tailvalue)==0);
	}
	while(headvalue>-6){
		assert(list_push_front(l,headvalue)==0);
		headvalue--;
		assert(list_is_empty(l)==0);
	}
	list_print(l);

}

int list_init(List* l){
	l->head=NULL;
	l->tail=NULL;
	return 0;
}

void list_destroy(List* l){
	while(l->head!=NULL){
		Node* temp=l->head;
		l->head=l->head->next;
		free(temp);
	}
	l->tail=NULL;
}

int list_is_empty(const List* l){
	return (l->head==NULL);
}

int list_push_front(List* l, int value){
	Node *n=malloc(sizeof(Node));
	if (n==NULL){
		fprintf(stderr,"push front error: malloc failure\n");
		return -1;
	}
	n->value=value;
	n->next=l->head;
	if(list_is_empty(l)){
		l->tail=n;
	}	
	l->head=n;
	return 0;
}

int list_push_back(List* l, int value){
	Node *n=malloc(sizeof(Node));
	if (n==NULL){
		fprintf(stderr,"push back error: malloc failure\n");
		return -1;
	}
	n->value=value;
	n->next=NULL;
	if(list_is_empty(l)){
		l->head=n;
	}
	else{
		l->tail->next=n;
	}	
	l->tail=n;
	return 0;
}

Node* list_find(const List* l, int value){
	if (list_is_empty(l)) return NULL;
	Node* n=l->head;
	while(n->value!=value){
		n=n->next;
		if (n==NULL){
			return NULL;
		}
	}
	return n;
}

int list_delete_value(List* l, int value){
	if (list_is_empty(l)) return 0;
	Node* prev=NULL;
	Node* curr=l->head;
	while(curr->value!=value){
		prev=curr;
		curr=curr->next;
		if (curr==NULL){
			return 0;
		}
	}
	if(curr==l->head){
		l->head=curr->next;
	}
	else if(curr==l->tail){
		prev->next=NULL;
		l->tail=prev;
	}
	else{
		prev->next=curr->next;
	}
	free(curr);
	return 1;
}	

void list_print(const List* l){
	Node* n=l->head;
	while(n!=NULL){
		printf("%d, ",n->value);
		n=n->next;
	}
	printf("\n");
}

int main(void){
	List l;
	run_tests(&l);
	list_destroy(&l);
	return 0;
}
