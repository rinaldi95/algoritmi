#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H


typedef struct DNode{
	int value;
	struct DNode* prev;
	struct DNode* next;
}DNode;

typedef struct{
	DNode* head;
	DNode* tail;
}DList;

// Initialize an empty list; sets head and tail to NULL
int dlist_init(DList* l);
// Free all nodes and reset head and tail to NULL
void dlist_destroy(DList* l);
// Return 1 if the list is empty, else 0
int dlist_is_empty(const DList* l);
// Insert a new value at the front; return 0 on success, non-zero on allocation failure
int dlist_push_front(DList* l, int value);
// Insert a new value at the back; return 0 on success, non-zero on allocation failure
int dlist_push_back(DList* l, int value);
// Find the first node matching value; return pointer or NULL if not found
DNode* dlist_find(const DList* l, int value);
// Insert a new value after the given node; return 0 on success, non-zero on allocation failure or if node is NULL
int dlist_insert_after(DList* l, DNode* node, int value);
// Delete the first node matching value; return 1 if deleted, 0 if not found
int dlist_delete_value(DList* l, int value);
// Print list contents forward for debugging/testing
void dlist_print_forward(const DList* l);
// Print list contents backward for debugging/testing
void dlist_print_backward(const DList* l);

#endif
