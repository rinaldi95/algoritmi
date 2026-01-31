#ifndef BST_H
#define BST_H

/*
 * Binary Search Tree (BST) for integer keys.
 *
 * This header is intentionally detailed: it defines the data structures,
 * the exact behavior of each operation, edge cases, and invariants.
 *
 * Key policy:
 * - Keys are unique. Insert of an existing key does NOT create a new node.
 * - Instead, insert reports "duplicate" and leaves the tree unchanged.
 *
 * Invariants:
 * - For every node N, all keys in N->left are strictly less than N->key.
 * - For every node N, all keys in N->right are strictly greater than N->key.
 * - size is the number of nodes currently stored in the tree.
 *
 * Complexity (h = tree height):
 * - search/insert/delete: O(h)
 * - worst-case h = n (degenerate tree), best-case h = log n (balanced)
 */

#include <stddef.h>

typedef struct BSTNode {
	int key;
	struct BSTNode *left;
	struct BSTNode *right;
} BSTNode;

typedef struct {
	BSTNode *root;
	int size;
} BSTree;

/*
 * Initialize an empty tree.
 *
 * Postconditions:
 * - tree->root == NULL
 * - tree->size == 0
 */
void bst_init(BSTree *tree);

/*
 * Destroy the tree and free all nodes.
 *
 * Postconditions:
 * - tree->root == NULL
 * - tree->size == 0
 * - Safe to call on an already-empty tree.
 */
void bst_destroy(BSTree *tree);

/*
 * Return 1 if the tree is empty, 0 otherwise.
 */
int bst_is_empty(const BSTree *tree);

/*
 * Return the number of nodes stored in the tree.
 */
int bst_size(const BSTree *tree);

/*
 * Search for a key.
 *
 * Returns:
 * - 1 if the key is found
 * - 0 otherwise
 */
int bst_contains(const BSTree *tree, int key);

/*
 * Insert a key into the tree.
 *
 * Returns:
 * - 1 if the key was inserted (tree changed)
 * - 0 if the key was a duplicate (tree unchanged)
 * - -1 on allocation failure (tree unchanged)
 *
 * Notes:
 * - Only one node per key is stored.
 * - size increases only when a new node is created.
 */
int bst_insert(BSTree *tree, int key);

/*
 * Remove a key from the tree.
 *
 * Returns:
 * - 1 if the key was found and removed
 * - 0 if the key was not present (tree unchanged)
 *
 * Delete cases:
 * - Leaf node: remove directly.
 * - One child: replace node by its child.
 * - Two children: replace node's key with its in-order successor's key,
 *   then delete the successor node.
 */
int bst_remove(BSTree *tree, int key);

/*
 * In-order traversal.
 *
 * The visitor is called once per node in ascending key order.
 * The callback receives the key and the user-provided context pointer.
 *
 * If visitor is NULL, traversal does nothing.
 */
void bst_in_order(const BSTree *tree,
	 void (*visit)(BSTNode *node, void *ctx), void *ctx);

/*
 * Pre-order traversal.
 *
 * Useful for serialization or copying structure.
 * If visitor is NULL, traversal does nothing.
 */
void bst_pre_order(const BSTree *tree, void (*visit)(BSTNode *node, void *ctx), void *ctx);
/*
 * Post-order traversal.
 *
 * Useful for freeing nodes bottom-up.
 * If visitor is NULL, traversal does nothing.
 */
void bst_post_order(const BSTree *tree,
	 void (*visit)(BSTNode *node, void *ctx), void *ctx);
/*
 * Optional helper: find the minimum key in the tree.
 *
 * Returns:
 * - 1 if the tree is non-empty (writes result to *out_key)
 * - 0 if the tree is empty (out_key unchanged)
 */
int bst_min(const BSTree *tree, int *out_key);

/*
 * Optional helper: find the maximum key in the tree.
 *
 * Returns:
 * - 1 if the tree is non-empty (writes result to *out_key)
 * - 0 if the tree is empty (out_key unchanged)
 */
int bst_max(const BSTree *tree, int *out_key);

#endif
