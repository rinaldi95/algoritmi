#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<math.h>

#include"rbt.h"

/*=========================helpers====================*/

typedef struct{
	int esit;
	int black_count;
	int max_high;
}ValidateCtx;

static void print_node(RBTNode *node, void *ctx){
	(void) ctx;
	printf("key=%d, color=%d, address=%p\n parent=%p, left=%p, right=%p\n",
			node->key,node->color,node, node->parent,node->left, node->right);
}

static int node_create(RBTree *tree, RBTNode **node,RBTNode *parent, int key){
	RBTNode *newnode=malloc(sizeof(RBTNode));
	if (newnode==NULL){
		fprintf(stderr,"Error: malloc failure. node_create not performed");
		return -1;
	}
	newnode->color=RBT_RED;
	newnode->key=key;
	newnode->left=tree->nil;
	newnode->right=tree->nil;
	newnode->parent=parent;	
	*node=newnode;
	if (parent!=tree->nil){	
		if (key<parent->key) parent->left=newnode;
		else parent->right=newnode;
	}
	tree->size++;
}

int rbt_init(RBTree *tree){
	tree->nil=malloc(sizeof(RBTNode));
	tree->root=tree->nil;
	tree->size=0;
	if (tree->nil==NULL){
		fprintf(stderr,"Error: malloc failure. rbt_init not performed");
		return -1;
	}
	tree->nil->parent=tree->nil;
	tree->nil->left=tree->nil;
	tree->nil->right=tree->nil;
	tree->nil->key=0;
	tree->nil->color=RBT_BLACK;
	return 1;
}

void destroy_node(RBTNode *node, void *ctx){
	(void) ctx;
	free(node);
}

void rbt_destroy(RBTree *tree){
	rbt_post_order(tree,destroy_node,NULL);
	tree->root=NULL;
	tree->size=0;
	free(tree->nil);
	tree->nil=NULL;
}

int rbt_is_empty(const RBTree *tree){
	return tree->size==0;
}

int rbt_size( const RBTree *tree){
	return tree->size;
}

int rbt_contains(const RBTree *tree, int key){
	if (rbt_is_empty(tree)) return 0;
	RBTNode *node=tree->root;
	while(node!=tree->nil){
		if (node->key>key) node=node->left;
		else if (node->key<key) node=node->right;
		else return 1;
	}
	return 0;
}

static void ll(RBTree *tree, RBTNode *g){
	printf("ll rotation \n");
	RBTNode *p= g->left;
	RBTNode *gg=g->parent;
	g->left=p->right;
	p->right=g;
	g->parent=p;
	p->parent=gg;
	if(g->left!=tree->nil)g->left->parent=g;
	if (g==tree->root) tree->root=p;
	else {
		if (g->key<gg->key) gg->left=p;
		else gg->right =p;
	} 
}

static void rr(RBTree *tree, RBTNode *g){
	printf("rr rotation \n");
	RBTNode *p= g->right;
	RBTNode *gg=g->parent;
	g->right=p->left;
	p->left=g;
	g->parent=p;
	p->parent=gg;
	if (g->right!=tree->nil) g->right->parent=g;
	if (g==tree->root) tree->root=p;
	else {
		if (g->key<gg->key) gg->left=p;
		else gg->right =p;
	} 

}

static void lr(RBTree *tree, RBTNode *g){
	printf("lr rotation on node key%d \n",g->key);
	printf("situazione precedente:\n\n");
	rbt_pre_order(tree,print_node,NULL);
	rr(tree,g->left);
	printf("situazione dopo rr:\n\n");
	rbt_pre_order(tree,print_node,NULL);
	
	ll(tree,g);
	
	printf("situazione finale:\n\n");
	rbt_pre_order(tree,print_node,NULL);
	printf("\n\n\n");
	printf("\n\n\n");
} 

static void rl(RBTree *tree, RBTNode *g){
	printf("rl rotation \n");
	ll(tree,g->right);
	rr(tree,g);
}


int insert_iteration( RBTree *tree, RBTNode *node, int key){
	int esit;
	if (key<node->key){
		if (node->left==tree->nil){
			esit= node_create(tree,&node->left, node,key);
		}
		else {
			esit= insert_iteration(tree, node->left, key);
		}
		if (esit){
			if (node->color==RBT_BLACK) return esit;
			else if (node->left->color==RBT_RED){
				printf("red red violation\n");
				if (node->parent->key>node->key) {
					RBTNode *bro=node->parent->right;
					if (bro->color==RBT_BLACK){
						node->color=RBT_BLACK;
						node->parent->color=RBT_RED;
						ll(tree,node->parent);
					}
					else{
						node->parent->color=RBT_RED;
						node->color=RBT_BLACK;
						bro->color=RBT_BLACK;
					}
				}
				else {
					RBTNode *bro =node->parent->left;
					if (bro->color==RBT_BLACK){
						node->left->color=RBT_BLACK;
						node->parent->color=RBT_RED;
						rl(tree,node->parent);				
					}
					else{
						node->parent->color=RBT_RED;
						node->color=RBT_BLACK;
						bro->color=RBT_BLACK;
					}
				}
			}
			tree->root->color=RBT_BLACK;
		}
		return esit;
	}
	else if( key>node->key){
		if (node->right==tree->nil){
			esit= node_create(tree,&node->right, node,key);
		}
		else esit= insert_iteration(tree, node->right, key);
		if (esit){
			if (node->color==RBT_BLACK) return esit;
			else if(node->right->color==RBT_RED){
				printf("red red violation\n");
				if (node->parent->key>node->key) {
					RBTNode *bro=node->parent->right;
					if (bro->color==RBT_BLACK){
						node->right->color=RBT_BLACK;
						node->parent->color=RBT_RED;
						lr(tree,node->parent);
					}
					else{
						node->parent->color=RBT_RED;
						node->color=RBT_BLACK;
						bro->color=RBT_BLACK;
					}
				}
				else{
					 RBTNode *bro =node->parent->left;
					if (bro->color==RBT_BLACK){
						node->color=RBT_BLACK;
						node->parent->color=RBT_RED;
						rr(tree,node->parent);
					}
					else{
						node->parent->color=RBT_RED;
						node->color=RBT_BLACK;
						bro->color=RBT_BLACK;
					}
				}
			}
			tree->root->color=RBT_BLACK;				
		}
		return esit;
	}
	else return 0;
}
			

int rbt_insert(RBTree *tree, int key){
	if (rbt_is_empty(tree)){
		if (node_create(tree,&tree->root,tree->nil, key)){
			tree->root->color=RBT_BLACK;
			return 1;
		}
		else return -1;
	}
	return insert_iteration(tree,tree->root, key);
}

int remove_min(RBTree *tree,RBTNode *node){
	printf("remove min\n");
	while(node->left!=tree->nil){
		node=node->left;
	}
	int result=node->key;
	remove_node(tree,node);
	return result;
	
}

void fixup(RBTree *tree, RBTNode *x, int left){
	printf("fixup\n");
	if(x==tree->root) return;
	if (x->color == RBT_RED) { 
		x->color = RBT_BLACK; 
		return; 
	}
	RBTNode *parent=x->parent;
	RBTNode *w=parent->left;
	RBTNode *near=w->right;
	RBTNode *far=w->left;
	if (left){		
		w=parent->right;
		near=w->left;
		far=w->right;
	}
	if (w->color==RBT_RED){
		w->color=RBT_BLACK;
		parent->color=RBT_RED;
		if (left) rr(tree,parent);
		else ll (tree,parent);
		fixup(tree,x,left);
	}
	else {
		if (near->color==RBT_BLACK&&far->color==RBT_BLACK){
			int new_left=0;
			if (w!=tree->nil)w->color=RBT_RED;
			if (parent==parent->parent->left)new_left=1;
			fixup(tree, parent,new_left);
		}
		else if (near->color==RBT_RED&& far->color==RBT_BLACK){
			near->color=RBT_BLACK;
			if (w!=tree->nil)w->color=RBT_RED;
			if (left) ll(tree,w);
			else rr(tree,w);
			x->color=RBT_BLACK;
			if(x->parent->left==x){
				w=parent->right;
				far=w->right;
				near=w->left;
			}
			else {
				w=parent->left;
				far=w->left;
				near=w->right;
			}
		}
		else if(far->color==RBT_RED){
			if (w!=tree->nil) w->color=w->parent->color;
			parent->color=RBT_BLACK;
			far->color=RBT_BLACK;
			if (left) rr(tree,parent);
			else ll(tree,parent);
			x->color=RBT_BLACK;
		}
	}
	tree->nil->parent=tree->nil;
}
	
			
				
static void replace_parent_link(RBTree *tree, RBTNode *node, RBTNode *child){
	if (node->parent == tree->nil){
		tree->root = child;
		child->parent = tree->nil;
	} else if (node->parent->left == node){
		node->parent->left = child;
		child->parent = node->parent;
	} else {
		node->parent->right = child;
		child->parent = node->parent;
	}
}
		
int remove_node(RBTree *tree,RBTNode *node){
	printf("remove node\n");
	if (node->color==RBT_RED){
		if (node->left==tree->nil){
			if(node->right==tree->nil){
				replace_parent_link(tree, node, tree->nil);
			}
			else {
				replace_parent_link(tree, node, node->right);
 			}
 		}
		else {
			if(node->right==tree->nil){
				replace_parent_link(tree, node, node->left);
			}
			else {	
				node->key=remove_min(tree,node->right);
			}
		}
	}
	else{
		if (node->left==tree->nil){
			RBTNode *x = node->right;
			replace_parent_link(tree, node, x);
			if (x->color==RBT_RED) x->color=RBT_BLACK;
 			else {
				int left = (x->parent != tree->nil && x->parent->left == x);
				fixup(tree, x, left);
			}
 		}
		else {
			if(node->right==tree->nil){
				RBTNode *x = node->left;
				replace_parent_link(tree, node, x);
				if (x->color==RBT_RED) x->color=RBT_BLACK;
	 			else {
					int left = (x->parent != tree->nil && x->parent->left == x);
					fixup(tree, x, left);
				}
			}
			else {	
				node->key=remove_min(tree,node->right);
			}
		}
	}
	return 1;
}
		
 			
 


int remove_iteration(RBTree *tree,RBTNode *node, int key){
	printf("remove iteraction\n");	
	if(node!=tree->nil){
		if (node->key>key){
			if (node->left==tree->nil)return 0;
			remove_iteration(tree,node->left,key);
		}
		else if (node->key<key) {
			if (node->right==tree->nil)return 0;
			remove_iteration(tree,node->right,key);
		}
		else return remove_node(tree,node);
	}
}

int rbt_remove(RBTree *tree, int key){
	printf("remove\n");
	if (rbt_is_empty(tree)) return 0;
	if( remove_iteration(tree,tree->root,key)){
		tree->size--;
	}
}

void in_order_iteration(const RBTree *tree, RBTNode *node,
		void (*visit)(RBTNode *node, void *ctx), void *ctx){
	if (node->left!=tree->nil) in_order_iteration(tree,node->left,visit,ctx);
	visit(node,ctx);
	if (node->right!=tree->nil) in_order_iteration(tree,node->right,visit,ctx);
}
void rbt_in_order(const RBTree *tree,
		void (*visit)(RBTNode *node, void *ctx), void *ctx){
	if (rbt_is_empty(tree)) return;
	in_order_iteration(tree,tree->root, visit, ctx);
}



void pre_order_iteration(const RBTree *tree, RBTNode *node,
		void (*visit)(RBTNode *node, void *ctx), void *ctx){
	visit(node,ctx);
	if (node->left!=tree->nil) pre_order_iteration(tree,node->left,visit,ctx);
	if (node->right!=tree->nil) pre_order_iteration(tree,node->right,visit,ctx);
}
void rbt_pre_order(const RBTree *tree,
		void (*visit)(RBTNode *node, void *ctx), void *ctx){
	printf("\n");
	if (rbt_is_empty(tree)) return;
	pre_order_iteration(tree,tree->root, visit, ctx);
}

void post_order_iteration(const RBTree *tree, RBTNode *node,
		void (*visit)(RBTNode *node, void *ctx), void *ctx){
	if (node->left!=tree->nil) post_order_iteration(tree,node->left,visit,ctx);
	if (node->right!=tree->nil) post_order_iteration(tree,node->right,visit,ctx);
	visit(node,ctx);
}
void rbt_post_order(const RBTree *tree,
		void (*visit)(RBTNode *node, void *ctx), void *ctx){
	if (rbt_is_empty(tree)) return;
	post_order_iteration(tree,tree->root, visit, ctx);
}

void validate_node(RBTNode *node, void *ctx){
	ValidateCtx *v= (ValidateCtx *)ctx;
	if (node->color==RBT_RED){
		if (node->parent->color==RBT_RED 
		|| node->left->color==RBT_RED
		|| node->right->color==RBT_RED){
			print_node(node, NULL);
			v->esit=0;
			printf("Error: double red\n");
		}
	}
	if (node->left==node->right){
		RBTNode *prev=node;
		int black_count=0;
		int high=0;
		while(prev->parent->left!=prev->parent->right){
			if (prev->color==RBT_BLACK){
				black_count++;
			}
			high++;
			prev=prev->parent;
		}
		if (high>v->max_high) v->max_high=high;
		printf("black_count=%d\n",black_count);
		if (v->black_count!=0){
			if (v->black_count!=black_count) {
				v->esit=0;
				printf("Error: black count error\n");
			}
			v->black_count=black_count;
		}
	}			
}

int rbt_validate(const RBTree *tree){
	if (tree->root->color!=RBT_BLACK){
		print_node(tree->nil, NULL);
		printf("root is not black\n");
		return 0;
	}
	if (tree->nil->color!=RBT_BLACK){
		print_node(tree->nil, NULL);
		printf("nil is not black\n");
		return 0;
	}
	print_node(tree->nil,NULL);
	ValidateCtx ctx;
	ctx.esit=1;
	ctx.black_count=0;
	ctx.max_high=0;
	rbt_pre_order(tree,validate_node,&ctx);
	printf("high=%d, size=%d \n",ctx.max_high, tree->size);
	if (ctx.max_high>(int)(2*log2(rbt_size(tree)+1))){
		ctx.esit=0;
		printf("tree is longer than expected\n");
	}
	return ctx.esit;
}


void run_tests(RBTree *tree){
	assert(rbt_init(tree));
	assert(rbt_is_empty(tree));
	rbt_pre_order(tree,print_node,NULL);

	for (int i=41; i<100; i++){
		int new=((i*i*i)<<4)%83;
		printf("new value:%d\n",new);
		rbt_insert(tree, new);
		rbt_pre_order(tree,print_node,NULL);
		assert(rbt_validate(tree));
	}
	assert(rbt_contains(tree,22));
	assert(rbt_contains(tree,23));
	assert(rbt_contains(tree,24));
	assert(rbt_contains(tree,-1)==0);

	printf("\n\n==============================remove test=======\n\n\n");
	for (int i=41; i<100; i++){
		int new=((i*i*i)<<4)%83;
		printf("remove value:%d\n",new);
		rbt_remove(tree, new);
		rbt_pre_order(tree,print_node,NULL);
		printf("removed value:%d\n",new);
		assert(rbt_validate(tree));
		printf("tree is valid");
	}
	rbt_destroy(tree);
	assert(rbt_is_empty(tree));
	rbt_init(tree);
	printf("All tests passed\n");
}


int main(void){
	RBTree tree;
	run_tests(&tree);
	rbt_destroy(&tree);
	return 0;
}
