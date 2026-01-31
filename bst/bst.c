#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>

#include"bst.h"

void print_node(BSTNode *node, void *ctx){
	(void)ctx;
	printf("key=%d, node=%p, left=%p, right=%p\n",
	 node->key, (void*)node, (void*)node->left, (void*)node->right);
}

void bst_init(BSTree *tree){
	tree->root = NULL;
	tree->size = 0;
}

void bst_destroy(BSTree *tree){
	BSTNode	*node,*prev=NULL;
	node=tree->root;
	while(tree->root!=NULL){
		if (node->left!=NULL){
			prev=node;
			node=node->left;
		}
		else if (node->right!=NULL){
			prev=node;
			node=node->right;
		}
		else {
			if (prev!=NULL){
				if (prev->key>node->key){
					prev->left=NULL;
				}
				else{
					prev->right=NULL;
				}
			}
			else{
				tree->root=NULL;
			}
			tree->size--;
			free(node);
			prev=NULL;
			node=tree->root;			
		}
	}
}

int bst_is_empty(const BSTree *tree){
	if (tree->size==0) return 1;
	else return 0;
}

int bst_size(const BSTree *tree){
	return tree->size;
}

int bst_contains(const BSTree *tree, int key){
	if (bst_is_empty(tree)){
		return 0;
	}
	BSTNode *node=tree->root;
	for(;;){
		if (key<node->key){
			if (node->left==NULL) return 0;
			node=node->left;
		}
		else if(key>node->key){
			if (node->right==NULL) return 0;
			node=node->right;
		}
		else{
			return 1;
		}
	}
}

void bst_node_create(BSTNode *node, int key){
	node->key=key;
	node->left=NULL;
	node->right=NULL;
}
		

int bst_insert(BSTree *tree, int key){
	if (bst_is_empty(tree)){
		tree->root=malloc(sizeof(BSTNode));
		if (tree->root==NULL){
			fprintf(stderr,
			"Error: malloc failure. bst_insert not performed\n");
			return -1;
		}
		bst_node_create(tree->root, key);
		tree->size++;
		return 1;
	}
	BSTNode *node=tree->root;
	for(;;){
		if (key<node->key){
			if (node->left==NULL) {
				node->left=malloc(sizeof(BSTNode));
				if (node->left==NULL){
					fprintf(stderr,
					"Error: malloc failure. bst_insert not performed\n");
					return -1;
				}
				bst_node_create(node->left,key);
				tree->size++;
				return 1;
			}
			node=node->left;
		}
		else if(key>node->key){
			if (node->right==NULL) {
				node->right=malloc(sizeof(BSTNode));
				if (node->right==NULL){
					fprintf(stderr,
					"Error: malloc failure. bst_insert not performed\n");
					return -1;
				}
				bst_node_create(node->right,key);
				tree->size++;
				return 1;
			}
			node=node->right;
		}
		else{
			return 0;
		}
	}
}

int bst_remove(BSTree *tree, int key){
	if (bst_is_empty(tree)){
		return 0;
	}
	BSTNode *node=tree->root;
	BSTNode *prev=NULL;
	for(;;){
		if (key<node->key){
			if (node->left==NULL) return 0;
			prev=node;
			node=node->left;
		}
		else if(key>node->key){
			if (node->right==NULL) return 0;
			prev=node;
			node=node->right;
		}
		else{
			if (node->left!=NULL){
				if (node->right==NULL){
					BSTNode *temp=node->left;
					*node=*(node->left);
					free(temp);
				}
				else{
					BSTNode *next=node->right;
					while (next->left!=NULL){
						next=next->left;
					}
					node->key=next->key;
					BSTree temp=*tree;
					temp.root=node->right;
					bst_remove(&temp,node->key);
					node->right=temp.root;
				}
			}
			else if(node->right==NULL){
				if (prev==NULL){
					tree->root=NULL;
				}
				else{
					if(prev->key>node->key) prev->left=NULL;
					else prev->right=NULL;
				}
				free(node);
			}
			else{
				BSTNode *temp=node->right;
				*node=*(node->right);
				free(temp);
			}
			tree->size--;
			return(1);
		}			
	}
}
	
	

void bst_in_order(const BSTree *tree, void (*visit)(BSTNode *node,
 void *ctx), void *ctx){
	if (bst_is_empty(tree)||(visit==NULL)) return;
	BSTNode *node=tree->root;
	BSTNode *prev=NULL;
	BSTNode *temp=NULL;
	int key;
	bst_min(tree,&key);
	while (1){
		if ((node->left!=NULL)&&(key<=node->left->key)){
			temp=prev;
			prev=node;
			node=node->left;
			prev->left=temp;
		}
		else if((node->right!=NULL)&&(key<=node->right->key)){
			visit(node,ctx);
			key=node->key+1;
			temp=prev;
			prev=node;
			node=node->right;
			prev->right=temp;
		}
		else{
			if(key<=node->key){
				visit(node,ctx);
				key=node->key+1;
			}
			temp=node;
			node=prev;
			if (prev==NULL) return;
			if(key<=prev->key){
				prev=node->left;
				node->left=temp;
			}
			else{
				prev=node->right;
				node->right=temp;
			}
		}
		
	}
}

void recursive_pre_order(const BSTree *tree, BSTNode *node,
		 void (*visit)(BSTNode *node, void *ctx), void *ctx){
	visit(node,ctx);
	if (node->left!=NULL){
		recursive_pre_order(tree, node->left,visit, ctx);
	}
	if (node->right!=NULL){
		recursive_pre_order(tree, node->right, visit, ctx);
	}
}

	
void bst_pre_order(const BSTree *tree,void (*visit)(BSTNode *node, void *ctx), void *ctx){
	if (bst_is_empty(tree)||(visit==NULL)) return;
	recursive_pre_order(tree, tree->root, visit, ctx);
}

void recursive_post_order(const BSTree *tree, BSTNode *node,
		 void (*visit)(BSTNode *node, void *ctx), void *ctx){
	if (node->left!=NULL){
		recursive_post_order(tree, node->left,visit, ctx);
	}
	if (node->right!=NULL){
		recursive_post_order(tree, node->right, visit, ctx);
	}
	visit(node,ctx);
}

	
void bst_post_order(const BSTree *tree,void (*visit)(BSTNode *node, void *ctx), void *ctx){
	if (bst_is_empty(tree)||(visit==NULL)) return;
	recursive_post_order(tree, tree->root, visit, ctx);
}

	
int bst_min(const BSTree *tree, int *out_key){
	if (bst_is_empty(tree)) return 0;
	BSTNode *node=tree->root;
	while(node->left!=NULL){
		node=node->left;
	}
	*out_key=node->key;
	return 1;
}


int bst_max(const BSTree *tree, int *out_key){
	if (bst_is_empty(tree)) return 0;
	BSTNode *node=tree->root;
	while(node->right!=NULL){
		node=node->right;
	}
	*out_key=node->key;
	return 1;
}
	

void tests(BSTree *tree){
	bst_init(tree);
	bst_in_order(tree, print_node, NULL);
	assert(bst_insert(tree,5));
	assert(bst_insert(tree,5)==0);
	bst_in_order(tree, print_node, NULL);
	assert(bst_insert(tree,3));
	assert(bst_insert(tree,6));
	assert(bst_insert(tree,5)==0);
	assert(bst_insert(tree,3)==0);
	assert(bst_insert(tree,6)==0);
	bst_in_order(tree, print_node, NULL);
	bst_in_order(tree, print_node, NULL);
	bst_destroy(tree);
	assert(bst_is_empty(tree));
	printf("test random\n");
	for (int i=21;i<50;i++){
		printf("new=%d\n",(i*i*i>>4)%100);
		bst_insert(tree,(i*i*i>>4)%100);
		bst_in_order(tree, print_node, NULL);
		printf("============================\n");
		printf("============================\n");	
	}

	printf("=============test pre order==============\n");
	bst_pre_order(tree, print_node,NULL);
	printf("=============test post order==============\n");
	bst_post_order(tree, print_node,NULL);
	
	printf("test remove\n\n\n");
	for (int i=101; !bst_is_empty(tree);i++){
		printf("removing %d\n", (i*i*i>>4)%100);
		if(bst_contains(tree,(i*i*i>>4)%100)){			
			assert(bst_remove(tree,(i*i*i>>4)%100)==1);
			printf("new size=%d\n",tree->size);
			bst_in_order(tree,print_node,NULL);
			
		}
		else{
			assert(bst_remove(tree,(i*i*i>>4)%100)==0);
		 	printf("key not present\n");
		}	
	}
	bst_destroy(tree);
	assert(bst_is_empty(tree));
	printf("test random\n");
	for (int i=9;i<20;i++){
		printf("new=%d\n",(i*i*i>>4)%100);
		bst_insert(tree,(i*i*i>>4)%100);
		bst_in_order(tree, print_node, NULL);
		printf("============================\n");
		printf("============================\n");	
	}

	
	printf("=============test pre order==============\n");
	bst_pre_order(tree, print_node,NULL);
	printf("=============test post order==============\n");
	bst_post_order(tree, print_node,NULL);
	printf("test remove\n\n\n");
	for (int i=101; !bst_is_empty(tree);i++){
		printf("removing %d\n", (i*i*i>>4)%100);
		if(bst_contains(tree,(i*i*i>>4)%100)){			
			assert(bst_remove(tree,(i*i*i>>4)%100)==1);
			bst_in_order(tree,print_node,NULL);
			printf("new size=%d\n",tree->size);
		}
		else{
			assert(bst_remove(tree,(i*i*i>>4)%100)==0);
		 	printf("key not present\n");
		}		
	}
	bst_destroy(tree);
	assert(bst_is_empty(tree));
	printf("test random\n");
	for (int i=10;i>=0;i--){
		printf("new=%d\n",(i));
		bst_insert(tree,i);
		bst_in_order(tree, print_node, NULL);
		printf("============================\n");
		printf("============================\n");	
	}

	printf("=============test pre order==============\n");
	bst_pre_order(tree, print_node,NULL);
	printf("=============test post order==============\n");
	bst_post_order(tree, print_node,NULL);
	
	printf("test remove\n\n\n");
	for (int i=101; !bst_is_empty(tree);i++){
		printf("removing %d\n", (i*i*i>>4)%100);
		if(bst_contains(tree,(i*i*i>>4)%100)){			
			assert(bst_remove(tree,(i*i*i>>4)%100)==1);
			bst_in_order(tree,print_node,NULL);
			printf("new size=%d\n",tree->size);
		}
		else{
			assert(bst_remove(tree,(i*i*i>>4)%100)==0);
		 	printf("key not present\n");
		}		
	}
	bst_destroy(tree);
	assert(bst_is_empty(tree));
	printf("test random\n");

	for (int i=56;i<70;i++){
		printf("new=%d\n",(i*i*i>>4)%100);
		bst_insert(tree,(i*i*i>>4)%100);
		bst_in_order(tree, print_node, NULL);
		printf("============================\n");
		printf("============================\n");	
	}
	printf("=============test pre order==============\n");
	bst_pre_order(tree, print_node,NULL);
	printf("=============test post order==============\n");
	bst_post_order(tree, print_node,NULL);
		
	
	printf("test remove\n\n\n");
	for (int i=70; !bst_is_empty(tree);i--){
		printf("removing %d\n", (i*i*i>>4)%100);
		if(bst_contains(tree,(i*i*i>>4)%100)){			
			assert(bst_remove(tree,(i*i*i>>4)%100)==1);
			bst_in_order(tree,print_node,NULL);
			printf("new size=%d\n",tree->size);
		}
		else{
			assert(bst_remove(tree,(i*i*i>>4)%100)==0);
		 	printf("key not present\n");
		}
	}
	bst_destroy(tree);
	assert(bst_is_empty(tree));
	assert(bst_contains(tree,0)==0);
	assert(bst_remove(tree,0)==0);
	bst_in_order(tree,print_node,NULL);
	bst_pre_order(tree,print_node,NULL);
	bst_post_order(tree,print_node,NULL);
	int out_value=100;
	assert(bst_min(tree,&out_value)==0);
	assert(out_value==100);
	assert(bst_max(tree,&out_value)==0);
	assert(out_value==100);
	assert(bst_insert(tree, 0)==1);
	assert(!bst_is_empty(tree));
	assert(bst_contains(tree,0)==1);
	bst_in_order(tree,print_node,NULL);
	bst_pre_order(tree,print_node,NULL);
	bst_post_order(tree,print_node,NULL);
	assert(bst_min(tree,&out_value)==1);
	assert(out_value==0);
	out_value=1;
	assert(bst_max(tree,&out_value)==1);
	assert(out_value==0);
	printf("All tests passed\n");
}


int main(void){
	BSTree tree;
	tests(&tree);
	return 0;
}





