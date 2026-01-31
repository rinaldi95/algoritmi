#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

static int max_int(int a, int b) { return (a > b) ? a : b; }

static int height(AVLNode *n) { return n ? n->height : 0; }

static void update_height(AVLNode *n) {
	if (n) n->height = 1 + max_int(height(n->left), height(n->right));
}

static int balance_factor(AVLNode *n) {
	return n ? height(n->left) - height(n->right) : 0;
}

static AVLNode *rotate_right(AVLNode *y) {
	AVLNode *x = y->left;
	AVLNode *t2 = x->right;

	x->right = y;
	y->left = t2;

	update_height(y);
	update_height(x);
	return x;
}

static AVLNode *rotate_left(AVLNode *x) {
	AVLNode *y = x->right;
	AVLNode *t2 = y->left;

	y->left = x;
	x->right = t2;

	update_height(x);
	update_height(y);
	return y;
}

static AVLNode *rebalance(AVLNode *n) {
	int bf = balance_factor(n);

	if (bf > 1) {
		if (balance_factor(n->left) < 0)
			n->left = rotate_left(n->left);
		return rotate_right(n);
	}
	if (bf < -1) {
		if (balance_factor(n->right) > 0)
			n->right = rotate_right(n->right);
		return rotate_left(n);
	}
	return n;
}

void avl_init(AVLTree *tree) {
	tree->root = NULL;
	tree->size = 0;
}

static void destroy_rec(AVLNode *node) {
	if (!node) return;
	destroy_rec(node->left);
	destroy_rec(node->right);
	free(node);
}

void avl_destroy(AVLTree *tree) {
	destroy_rec(tree->root);
	tree->root = NULL;
	tree->size = 0;
}

int avl_is_empty(const AVLTree *tree) { return tree->size == 0; }

int avl_size(const AVLTree *tree) { return tree->size; }

int avl_contains(const AVLTree *tree, int key) {
	AVLNode *cur = tree->root;
	while (cur) {
		if (key < cur->key)
			cur = cur->left;
		else if (key > cur->key)
			cur = cur->right;
		else
			return 1;
	}
	return 0;
}

static AVLNode *new_node(int key) {
	AVLNode *n = (AVLNode *)malloc(sizeof(AVLNode));
	if (!n) return NULL;
	n->key = key;
	n->height = 1;
	n->left = NULL;
	n->right = NULL;
	return n;
}

static AVLNode *insert_rec(AVLNode *node, int key, int *status) {
	if (!node) {
		AVLNode *n = new_node(key);
		if (!n) {
			*status = -1;
			return NULL;
		}
		*status = 1;
		return n;
	}

	if (key < node->key)
		node->left = insert_rec(node->left, key, status);
	else if (key > node->key)
		node->right = insert_rec(node->right, key, status);
	else {
		*status = 0;
		return node;
	}

	if (*status == -1) return node;

	update_height(node);
	return rebalance(node);
}

int avl_insert(AVLTree *tree, int key) {
	int status = 0;
	tree->root = insert_rec(tree->root, key, &status);
	if (status == 1) tree->size++;
	return status;
}

static AVLNode *min_node(AVLNode *node) {
	AVLNode *cur = node;
	while (cur && cur->left) cur = cur->left;
	return cur;
}

static AVLNode *remove_rec(AVLNode *node, int key, int *status) {
	if (!node) {
		*status = 0;
		return NULL;
	}

	if (key < node->key) {
		node->left = remove_rec(node->left, key, status);
	} else if (key > node->key) {
		node->right = remove_rec(node->right, key, status);
	} else {
		*status = 1;
		if (!node->left || !node->right) {
			AVLNode *tmp = node->left ? node->left : node->right;
			free(node);
			return tmp;
		} else {
			AVLNode *succ = min_node(node->right);
			node->key = succ->key;
			node->right = remove_rec(node->right, succ->key, status);
		}
	}

	if (!node) return NULL;
	update_height(node);
	return rebalance(node);
}

int avl_remove(AVLTree *tree, int key) {
	int status = 0;
	tree->root = remove_rec(tree->root, key, &status);
	if (status == 1) tree->size--;
	return status;
}

static void in_order_rec(AVLNode *node,
			 void (*visit)(AVLNode *node, void *ctx),
			 void *ctx) {
	if (!node) return;
	in_order_rec(node->left, visit, ctx);
	visit(node, ctx);
	in_order_rec(node->right, visit, ctx);
}

void avl_in_order(const AVLTree *tree,
		  void (*visit)(AVLNode *node, void *ctx), void *ctx) {
	if (!visit) return;
	in_order_rec(tree->root, visit, ctx);
}

static void pre_order_rec(AVLNode *node,
			  void (*visit)(AVLNode *node, void *ctx),
			  void *ctx) {
	if (!node) return;
	visit(node, ctx);
	pre_order_rec(node->left, visit, ctx);
	pre_order_rec(node->right, visit, ctx);
}

void avl_pre_order(const AVLTree *tree,
		   void (*visit)(AVLNode *node, void *ctx), void *ctx) {
	if (!visit) return;
	pre_order_rec(tree->root, visit, ctx);
}

static void post_order_rec(AVLNode *node,
			   void (*visit)(AVLNode *node, void *ctx),
			   void *ctx) {
	if (!node) return;
	post_order_rec(node->left, visit, ctx);
	post_order_rec(node->right, visit, ctx);
	visit(node, ctx);
}

void avl_post_order(const AVLTree *tree,
		    void (*visit)(AVLNode *node, void *ctx), void *ctx) {
	if (!visit) return;
	post_order_rec(tree->root, visit, ctx);
}

int avl_min(const AVLTree *tree, int *out_key) {
	if (!tree || !out_key || !tree->root) return 0;
	AVLNode *n = min_node(tree->root);
	*out_key = n->key;
	return 1;
}

int avl_max(const AVLTree *tree, int *out_key) {
	if (!tree || !out_key || !tree->root) return 0;
	AVLNode *cur = tree->root;
	while (cur->right) cur = cur->right;
	*out_key = cur->key;
	return 1;
}
