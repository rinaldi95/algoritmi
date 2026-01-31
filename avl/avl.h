#ifndef AVL_H
#define AVL_H

/*
 * AVL Tree (self-balancing BST) for integer keys.
 *
 * This header is intentionally detailed: it defines data structures,
 * required behavior, edge cases, invariants, and complexity expectations.
 *
 * Key policy:
 * - Keys are unique. Insert of an existing key does NOT create a new node.
 * - Insert reports "duplicate" and leaves the tree unchanged.
 *
 * AVL invariants:
 * - For every node N, all keys in N->left are strictly less than N->key.
 * - For every node N, all keys in N->right are strictly greater than N->key.
 * - height(N) = 1 + max(height(left), height(right)), with height(NULL) = 0.
 * - balance factor BF(N) = height(left) - height(right) must be in {-1, 0, +1}.
 * - size is the number of nodes currently stored in the tree.
 *
 * Complexity (n = number of nodes):
 * - search/insert/delete: O(log n) worst-case (AVL keeps height balanced)
 * - traversals: O(n)
 */

#include <stddef.h>

typedef struct AVLNode {
	int key;
	int height;           /* cached height of this node */
	struct AVLNode *left;
	struct AVLNode *right;
} AVLNode;

typedef struct {
	AVLNode *root;
	int size;
} AVLTree;

/*
 * Initialize an empty AVL tree.
 *
 * Postconditions:
 * - tree->root == NULL
 * - tree->size == 0
 */
void avl_init(AVLTree *tree);

/*
 * Destroy the tree and free all nodes.
 *
 * Postconditions:
 * - tree->root == NULL
 * - tree->size == 0
 * - Safe to call on an already-empty tree.
 */
void avl_destroy(AVLTree *tree);

/*
 * Return 1 if the tree is empty, 0 otherwise.
 */
int avl_is_empty(const AVLTree *tree);

/*
 * Return the number of nodes stored in the tree.
 */
int avl_size(const AVLTree *tree);

/*
 * Search for a key.
 *
 * Returns:
 * - 1 if the key is found
 * - 0 otherwise
 */
int avl_contains(const AVLTree *tree, int key);

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
 * - After insertion, the path from the inserted node to the root is rebalanced
 *   using rotations (LL, RR, LR, RL) so that all balance factors stay in {-1,0,+1}.
 */
int avl_insert(AVLTree *tree, int key);

/*
 * Remove a key from the tree.
 *
 * Returns:
 * - 1 if the key was found and removed
 * - 0 if the key was not present (tree unchanged)
 *
 * Delete cases (same as BST):
 * - Leaf node: remove directly.
 * - One child: replace node by its child.
 * - Two children: replace node's key with its in-order successor's key,
 *   then delete the successor node.
 *
 * After deletion, rebalance on the path back to the root (rotations if needed).
 */
int avl_remove(AVLTree *tree, int key);

/*
 * In-order traversal.
 *
 * The visitor is called once per node in ascending key order.
 * The callback receives the node pointer and the user-provided context pointer.
 *
 * If visitor is NULL, traversal does nothing.
 */
void avl_in_order(const AVLTree *tree,
		 void (*visit)(AVLNode *node, void *ctx), void *ctx);

/*
 * Pre-order traversal.
 *
 * Useful for serialization or copying structure.
 * If visitor is NULL, traversal does nothing.
 */
void avl_pre_order(const AVLTree *tree,
		 void (*visit)(AVLNode *node, void *ctx), void *ctx);

/*
 * Post-order traversal.
 *
 * Useful for freeing nodes bottom-up.
 * If visitor is NULL, traversal does nothing.
 */
void avl_post_order(const AVLTree *tree,
		 void (*visit)(AVLNode *node, void *ctx), void *ctx);

/*
 * Optional helper: find the minimum key in the tree.
 *
 * Returns:
 * - 1 if the tree is non-empty (writes result to *out_key)
 * - 0 if the tree is empty (out_key unchanged)
 */
int avl_min(const AVLTree *tree, int *out_key);

/*
 * Optional helper: find the maximum key in the tree.
 *
 * Returns:
 * - 1 if the tree is non-empty (writes result to *out_key)
 * - 0 if the tree is empty (out_key unchanged)
 */
int avl_max(const AVLTree *tree, int *out_key);

#endif
