#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include"avl.h"

static void print_node(AVLNode *node, void *ctx){
	(void) ctx;
	printf("k=%d, h=%d, node=%p, left=%p, right=%p\n",
			node->key,node->height,node,node->left,node->right);
}

void avl_init(AVLTree *tree){
	tree->root=NULL;
	tree->size=0;
}

static void destroy_iteration(AVLTree *tree, AVLNode *node){
	if (node->left!=NULL) destroy_iteration(tree, node->left);
	if (node->right!=NULL) destroy_iteration(tree, node->right);
	free(node);
	tree->size--;

}

void avl_destroy(AVLTree *tree){
	if (tree->root==NULL) return;
	destroy_iteration(tree, tree->root);
	tree->root=NULL;
	assert(tree->size==0); //debug
}

int avl_is_empty(const AVLTree *tree){
	if (tree->size==0) return 1;
	return 0;
}

int avl_size(const AVLTree *tree){
	return tree->size;
}

int avl_contains(const AVLTree *tree, int key){
	AVLNode *node=tree->root;
	while(node!=NULL){
		if (key<node->key) node=node->left;
		else if (key>node->key) node=node->right;
		else return 1;
	}
	return 0;
}

static int create_new_node(AVLTree *tree, AVLNode **node, int key){
	*node = malloc(sizeof(AVLNode));
	if (*node==NULL){
		fprintf(stderr,"Error: malloc failure. avl_insert not performed:");
		return -1;
	}
	(*node)->left=NULL;
	(*node)->right=NULL;
	(*node)->key=key;
	(*node)->height=1;
	tree->size++;
	return 1;
}	

static int height_calc(AVLNode *node){
	if (node==NULL)return 0;
	else if (node->right!=NULL&&node->left!=NULL){
	return (node->right->height>node->left->height 
			? node->right->height+1 : node->left->height+1);
	}
	else if (node->right!=NULL)return node->right->height+1;
	else if (node->left!=NULL) return node->left->height+1;
	else return 1;
}

static int node_weight(AVLNode *node){
	return height_calc(node->left)-height_calc(node->right);
}

static void ll(AVLNode *node){
	printf("ll on node %d\n",node->key);
	AVLNode *l=node->left;
	int swap=node->key;
	node->key=l->key;
	l->key=swap;
	node->left=l->left;
	l->left=l->right;
	l->right= node->right;
	node->right=l;
	node->left->height=height_calc(node->left);
	node->right->height=height_calc(node->right);
	node->height=height_calc(node);
}

static void rr(AVLNode *node){
	printf("rr on node %d\n",node->key);
	AVLNode *r=node->right;
	int swap=node->key;
	node->key=r->key;
	r->key=swap;
	node->right=r->right;
	r->right=r->left;
	r->left= node->left;
	node->left=r;
	node->left->height=height_calc(node->left);
	node->right->height=height_calc(node->right);
	node->height=height_calc(node);
}

static void lr(AVLNode *node){
	printf("lr on node %d\n",node->key);
	AVLNode *lr=node->left->right;
	int swap=node->key;
	node->key=lr->key;
	lr->key=swap;
	node->left->right=lr->left;
	lr->left=lr->right;
	lr->right= node->right;
	node->right=lr;
	node->left->height=height_calc(node->left);
	node->right->height=height_calc(node->right);
	node->height=height_calc(node);
}
	
	

static void rl(AVLNode *node){
	printf("rl on node %d\n",node->key);
	AVLNode *rl=node->right->left;
	int swap=node->key;
	node->key=rl->key;
	rl->key=swap;
	node->right->left=rl->right;
	rl->right=rl->left;
	rl->left= node->left;
	node->left=rl;
	node->left->height=height_calc(node->left);
	node->right->height=height_calc(node->right);
	node->height=height_calc(node);
	
}

static void rebalance(AVLNode *node){
  	printf("start rebalance node%p\n",node);
	if (node_weight(node)>1){
		if (node_weight((node)->left)>0){
			ll(node);
		}
		else{
			lr(node);
		}
	}	
	if (node_weight(node)<-1){
		if (node_weight((node)->right)<0){
			rr(node);
		}
		else {
			rl(node);
		}
	}
	printf("end rebalance\n");
}

static int insert_iteration(AVLTree *tree, AVLNode **node, int key){
	if (*node==NULL) return create_new_node(tree,node,key);
	else if (key<(*node)->key) {
		switch (insert_iteration(tree,&(*node)->left,key)){
		case 1:
			if ((*node)->height==(*node)->left->height){
				(*node)->height++;
			}
			if (node_weight(*node)>1){
				if (node_weight((*node)->left)>0){
					ll(*node);
				}
				else{
					lr(*node);
				}
			}			
			return 1;
		case 0:
			return 0;
		case -1:
			return -1;
		}
	}
	else if (key>(*node)->key) {
		switch (insert_iteration(tree,&(*node)->right,key)){
		case 1:
			if ((*node)->height==(*node)->right->height){
				(*node)->height++;
			}
			if (node_weight(*node)<-1){
				if (node_weight((*node)->right)<0){
					rr(*node);
				}
				else {
					rl(*node);
				}
			}	
			return 1;
		case 0:
			return 0;
		case -1:
			return -1;
		}
	}
	else return 0;
}

int avl_insert(AVLTree *tree, int key){
	return(insert_iteration(tree, &tree->root, key));
}

static void find_next_iteraction(AVLNode *node,AVLNode **output){
		printf("debug node=%p\n",node);
	if (node->left!=NULL){
		find_next_iteraction(node->left,output);
			printf("debug node=%p\n",node);
		if (node->left==*output) node->left=node->left->right;
		node->height=height_calc(node);
		rebalance(node);
			printf("debug node=%p\n",node);
	}
	else{
			printf("debug node=%p\n",node);
		*output=node;
			printf("debug node3=%p\n",node);

	}
}

static int remove_iteration(AVLNode *node,int key){
	printf("debug node=%p \n",node);
	if(node==NULL) return 0;
	else{
		if (key<node->key) {
			switch (remove_iteration(node->left,key)){
				case 0:
					return 0;
				case 1:
					node->height=height_calc(node);
					rebalance(node);
					return 1;
				case 2:
					node->left=NULL;
					node->height=height_calc(node);
					rebalance(node);
					return 1;
			}
		}
		else if(key>node->key) {
			switch (remove_iteration(node->right,key)){
				case 0:
					return 0;
				case 1:
					node->height=height_calc(node);
					printf("actually on node %p key %d\n",node,node->key); 
					  	AVLTree tree;
  	tree.root=node;
  	avl_pre_order(&tree,print_node,NULL);

					rebalance(node);
					return 1;
				case 2:
					node->right=NULL;
					node->height=height_calc(node);
					rebalance(node);
					return 1;
			}
		}
		else {
			printf("key %d found\n",node->key);
			if(node->right!=NULL){
				if (node->left!=NULL){
					AVLNode *next;	
					find_next_iteraction(node->right,&next);
						printf("debug nod5e=%p\n",node);
					node->key=next->key;
					if (node->right==next) node->right=next->right;
						printf("debug node=%p\n",node);
					rebalance(node);
					free(next);
				}
				else{
						printf("debugkuhi node=%p\n",node);	
					AVLNode *temp=node->right;				
					*node=*(node->right);
					free(temp);
				}
			}
			else{
				if(node->left!=NULL){
					AVLNode *temp=node->left;
					*node=*(node->left);
					free(temp);
				}
				else{
					free(node);
					return 2;
				}
			}
			node->height=height_calc(node);
			return 1;
		}
	}
}
		

int avl_remove(AVLTree *tree, int key){
	if (avl_size(tree)==1&&key==tree->root->key) {
		avl_destroy(tree);
		return 1;
	}
	if (remove_iteration(tree->root,key)!=0){
		tree->size--;
		return 1;
	}
	else return 0;
}

static void in_order_iteration(AVLNode *node,
		void(*visit)(AVLNode *node, void *ctx), void *ctx){
	if (node->left!=NULL) in_order_iteration(node->left,visit,ctx);
	visit(node,ctx);
	if (node->right!=NULL) in_order_iteration(node->right,visit,ctx);
}

void avl_in_order(const AVLTree *tree,
		void(*visit)(AVLNode *node, void *ctx), void *ctx){
	if ((*visit)==NULL) return;
	if(avl_is_empty(tree)) return;
	in_order_iteration(tree->root,visit,ctx);
}


static void pre_order_iteration(AVLNode *node,
		 void (*visit)(AVLNode *node, void *ctx), void *ctx){
	visit(node,ctx);
	
	if (node->left!=NULL) pre_order_iteration(node->left,visit,ctx);
	if (node->right!=NULL) pre_order_iteration(node->right,visit,ctx);	
}
		 
void avl_pre_order(const AVLTree *tree,
		 void (*visit)(AVLNode *node, void *ctx), void *ctx){
	if ((*visit)==NULL) return;
	if(avl_is_empty(tree))return;
	pre_order_iteration(tree->root,visit,ctx);
}


static void post_order_iteration(AVLNode *node,
		 void (*visit)(AVLNode *node, void *ctx), void *ctx){
	if (node->left!=NULL) post_order_iteration(node->left,visit,ctx);
	if (node->right!=NULL) post_order_iteration(node->right,visit,ctx);
	visit(node,ctx);	
}
		 
void avl_post_order(const AVLTree *tree,
		 void (*visit)(AVLNode *node, void *ctx), void *ctx){
	if ((*visit)==NULL) return;
	if(avl_is_empty(tree))return;
	post_order_iteration(tree->root,visit,ctx);
}

int avl_min(const AVLTree *tree, int *out_key){
	if (avl_is_empty(tree)) return 0;
	AVLNode *node=tree->root;
	while(node->left!=NULL){
		node=node->left;
	}
	*out_key=node->key;
	return 1;
}

int avl_max(const AVLTree *tree, int *out_key){
	if (avl_is_empty(tree)) return 0;
	AVLNode *node=tree->root;
	while(node->right!=NULL){
		node=node->right;
	}
	*out_key=node->key;
	return 1;
}		 

void tests(AVLTree *tree){
	avl_init(tree);	
	assert(avl_is_empty(tree));
	assert(avl_insert(tree,16)==1);
	assert(avl_contains(tree,16));
	assert(avl_is_empty(tree)==0);
	avl_pre_order(tree,print_node,NULL);
	avl_destroy(tree);
	assert(avl_contains(tree,16)==0);
	printf("\n=============test insert=========================\n\n");
	for (int i =21;i<40;i++){
		int new=(i*i<<4)%90;
		if (!avl_contains(tree,new)){
			printf("\n------insert value %d --------\n",new);
			assert(avl_insert(tree,new)==1);
			avl_pre_order(tree,print_node,NULL);
			avl_in_order(tree,print_node,NULL);
			avl_post_order(tree,print_node,NULL);
		}
		else {
			printf("\n--------value %d already present-----\n",new);
			assert(avl_insert(tree,new)==0);
		}
	}
	avl_destroy(tree);
	printf("\n======================test remove===============\n\n");

	for (int i =21;i<40;i++){
		int new=(i*i<<4)%90;
		if (avl_contains(tree,new)){
			printf("\n------remove value %d --------\n",new);
			assert(avl_remove(tree,new)==1);
			avl_pre_order(tree,print_node,NULL);
		}
		else {
			printf("\n--------value %d not present-----\n",new);
			assert(avl_remove(tree,new)==0);
		}
	}


		printf("\n=============test insert=========================\n\n");
		avl_pre_order(tree,print_node,NULL);
		printf("size %d, root %p",tree->size, tree->root);
	for (int i =101;i<140;i++){
		int new=(i*i<<4)%91;
		if (!avl_contains(tree,new)){
			printf("\n------insert value %d --------\n",new);
			assert(avl_insert(tree,new)==1);
			avl_pre_order(tree,print_node,NULL);
		}
		else {
			printf("\n--------value %d already present-----\n",new);
			assert(avl_insert(tree,new)==0);
		}
	}
	printf("\n======================test remove===============\n\n");

	for (int i =41;!avl_is_empty(tree);i++){
		int new=(i*i<<4)%91;
		if (avl_contains(tree,new)){
			printf("\n------remove value %d --------\n",new);
			assert(avl_remove(tree,new)==1);
			avl_pre_order(tree,print_node,NULL);
		}
		else {
			printf("\n--------value %d not present-----\n",new);
			assert(avl_remove(tree,new)==0);
		}
	}
	
	printf("all tests passed\n");
}


int main(void){
	AVLTree tree;
	tests(&tree);
	return 0;
}
