#include <stdlib.h>

#include "bst.h"

static BSTNode *bst_node_create(int key) {
	BSTNode *node = malloc(sizeof(*node));
	if (node == NULL) {
		return NULL;
	}
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void bst_init(BSTree *tree) {
	tree->root = NULL;
	tree->size = 0;
}

static void bst_destroy_nodes(BSTNode *node) {
	if (node == NULL) {
		return;
	}
	bst_destroy_nodes(node->left);
	bst_destroy_nodes(node->right);
	free(node);
}

void bst_destroy(BSTree *tree) {
	bst_destroy_nodes(tree->root);
	tree->root = NULL;
	tree->size = 0;
}

int bst_is_empty(const BSTree *tree) {
	return tree->size == 0;
}

int bst_size(const BSTree *tree) {
	return tree->size;
}

int bst_contains(const BSTree *tree, int key) {
	BSTNode *node = tree->root;
	while (node != NULL) {
		if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		} else {
			return 1;
		}
	}
	return 0;
}

int bst_insert(BSTree *tree, int key) {
	BSTNode *parent = NULL;
	BSTNode *node = tree->root;

	while (node != NULL) {
		if (key < node->key) {
			parent = node;
			node = node->left;
		} else if (key > node->key) {
			parent = node;
			node = node->right;
		} else {
			return 0;
		}
	}

	node = bst_node_create(key);
	if (node == NULL) {
		return -1;
	}

	if (parent == NULL) {
		tree->root = node;
	} else if (key < parent->key) {
		parent->left = node;
	} else {
		parent->right = node;
	}

	tree->size++;
	return 1;
}

static BSTNode *bst_detach_min(BSTNode *node, BSTNode **min_node) {
	if (node->left == NULL) {
		*min_node = node;
		return node->right;
	}
	node->left = bst_detach_min(node->left, min_node);
	return node;
}

static BSTNode *bst_remove_node(BSTNode *node, int key, int *removed) {
	if (node == NULL) {
		return NULL;
	}
	if (key < node->key) {
		node->left = bst_remove_node(node->left, key, removed);
		return node;
	}
	if (key > node->key) {
		node->right = bst_remove_node(node->right, key, removed);
		return node;
	}

	*removed = 1;
	if (node->left == NULL) {
		BSTNode *right = node->right;
		free(node);
		return right;
	}
	if (node->right == NULL) {
		BSTNode *left = node->left;
		free(node);
		return left;
	}

	{
		BSTNode *min_node = NULL;
		node->right = bst_detach_min(node->right, &min_node);
		node->key = min_node->key;
		free(min_node);
		return node;
	}
}

int bst_remove(BSTree *tree, int key) {
	int removed = 0;
	tree->root = bst_remove_node(tree->root, key, &removed);
	if (removed) {
		tree->size--;
	}
	return removed;
}

static void bst_in_order_visit(BSTNode *node, void (*visit)(BSTNode *, void *),
		void *ctx) {
	if (node == NULL) {
		return;
	}
	bst_in_order_visit(node->left, visit, ctx);
	visit(node, ctx);
	bst_in_order_visit(node->right, visit, ctx);
}

void bst_in_order(const BSTree *tree, void (*visit)(BSTNode *node, void *ctx),
		void *ctx) {
	if (tree->root == NULL || visit == NULL) {
		return;
	}
	bst_in_order_visit(tree->root, visit, ctx);
}

static void bst_pre_order_visit(BSTNode *node, void (*visit)(BSTNode *, void *),
		void *ctx) {
	if (node == NULL) {
		return;
	}
	visit(node, ctx);
	bst_pre_order_visit(node->left, visit, ctx);
	bst_pre_order_visit(node->right, visit, ctx);
}

void bst_pre_order(const BSTree *tree, void (*visit)(BSTNode *node, void *ctx),
		void *ctx) {
	if (tree->root == NULL || visit == NULL) {
		return;
	}
	bst_pre_order_visit(tree->root, visit, ctx);
}

static void bst_post_order_visit(BSTNode *node, void (*visit)(BSTNode *, void *),
		void *ctx) {
	if (node == NULL) {
		return;
	}
	bst_post_order_visit(node->left, visit, ctx);
	bst_post_order_visit(node->right, visit, ctx);
	visit(node, ctx);
}

void bst_post_order(const BSTree *tree, void (*visit)(BSTNode *node, void *ctx),
		void *ctx) {
	if (tree->root == NULL || visit == NULL) {
		return;
	}
	bst_post_order_visit(tree->root, visit, ctx);
}

int bst_min(const BSTree *tree, int *out_key) {
	BSTNode *node = tree->root;
	if (node == NULL) {
		return 0;
	}
	while (node->left != NULL) {
		node = node->left;
	}
	*out_key = node->key;
	return 1;
}

int bst_max(const BSTree *tree, int *out_key) {
	BSTNode *node = tree->root;
	if (node == NULL) {
		return 0;
	}
	while (node->right != NULL) {
		node = node->right;
	}
	*out_key = node->key;
	return 1;
}

void bst_in_order_morris(const BSTree *tree, void (*visit)(BSTNode *node, void *ctx),
		void *ctx) {
	BSTNode *cur = tree->root;

	if (cur == NULL || visit == NULL) {
		return;
	}

	while (cur != NULL) {
		if (cur->left == NULL) {
			visit(cur, ctx);
			cur = cur->right;
		} else {
			BSTNode *pred = cur->left;
			while (pred->right != NULL && pred->right != cur) {
				pred = pred->right;
			}
			if (pred->right == NULL) {
				pred->right = cur;
				cur = cur->left;
			} else {
				pred->right = NULL;
				visit(cur, ctx);
				cur = cur->right;
			}
		}
	}
}
