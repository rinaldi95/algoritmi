#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H


typedef struct Node{
	int value;
	struct Node* next;
}Node;

typedef struct{
	Node* head;
	Node* tail;
}List;

// Initialize an empty list; sets head and tail to NULL
int list_init(List* l);
// Free all nodes and reset head and tail to NULL
void list_destroy(List* l);
// Return 1 if the list is empty, else 0
int list_is_empty(const List* l);
// Insert a new value at the front; return 0 on success, non-zero on allocation failure
int list_push_front(List* l, int value);
// Insert a new value at the back; return 0 on success, non-zero on allocation failure
int list_push_back(List* l, int value);
// Find the first node matching value; return pointer or NULL if not found
Node* list_find(const List* l, int value);
// Delete the first node matching value; return 1 if deleted, 0 if not found
int list_delete_value(List* l, int value);
// Print list contents for debugging/testing
void list_print(const List* l);

#endif
