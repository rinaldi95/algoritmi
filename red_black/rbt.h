#ifndef RBT_H
#define RBT_H

/*
 * Red-Black Tree (RBT) for integer keys.
 *
 * This header is intentionally detailed: it defines the data structures,
 * the exact behavior of each operation, edge cases, and invariants.
 * Use it as a checklist while implementing rbt.c.
 *
 * Key policy:
 * - Keys are unique. Insert of an existing key does NOT create a new node.
 * - Instead, insert reports "duplicate" and leaves the tree unchanged.
 *
 * Sentinel policy (recommended and assumed by the API contracts):
 * - The tree owns a single shared NIL sentinel node (tree->nil).
 * - All external leaves are represented by tree->nil, not NULL.
 * - tree->nil is always BLACK.
 * - For an empty tree: tree->root == tree->nil.
 * - Do NOT free tree->nil until rbt_destroy.
 * - If rbt_init fails, tree->root and tree->nil are set to NULL.
 *
 * Red-Black invariants:
 * 1) Every node is either RED or BLACK.
 * 2) The root is BLACK.
 * 3) All leaves (tree->nil) are BLACK.
 * 4) No RED node has a RED child.
 * 5) For each node, all paths from that node to its descendant leaves
 *    contain the same number of BLACK nodes (black-height).
 *
 * BST invariants:
 * - For every node N, all keys in N->left are strictly less than N->key.
 * - For every node N, all keys in N->right are strictly greater than N->key.
 * - size is the number of non-NIL nodes currently stored in the tree.
 *
 * Complexity (n = number of nodes):
 * - search/insert/delete: O(log n) worst-case
 */

#include <stddef.h>

typedef enum {
	RBT_RED = 0,
	RBT_BLACK = 1
} RBTColor;

typedef struct RBTNode {
	int key;
	RBTColor color;
	struct RBTNode *parent;
	struct RBTNode *left;
	struct RBTNode *right;
} RBTNode;

typedef struct {
	RBTNode *root;
	RBTNode *nil; /* shared sentinel */
	int size;
} RBTree;

/*
 * Initialize an empty tree.
 *
 * Returns:
 * - 1 on success
 * - -1 on allocation failure
 *
 * Postconditions on success:
 * - tree->nil is allocated and colored BLACK
 * - tree->root == tree->nil
 * - tree->size == 0
 *
 * Postconditions on failure:
 * - tree->root == NULL
 * - tree->nil == NULL
 * - tree->size == 0
 */
int rbt_init(RBTree *tree);

/*
 * Destroy the tree and free all nodes (including the NIL sentinel).
 *
 * Postconditions:
 * - tree->root == NULL
 * - tree->nil == NULL
 * - tree->size == 0
 * - Safe to call on an already-empty tree or after a failed rbt_init.
 */
void rbt_destroy(RBTree *tree);

/*
 * Return 1 if the tree is empty, 0 otherwise.
 */
int rbt_is_empty(const RBTree *tree);

/*
 * Return the number of nodes stored in the tree (excluding NIL).
 */
int rbt_size(const RBTree *tree);

/*
 * Search for a key.
 *
 * Returns:
 * - 1 if the key is found
 * - 0 otherwise
 */
int rbt_contains(const RBTree *tree, int key);

/*
 * Insert a key into the tree.
 *
 * Returns:
 * - 1 if the key was inserted (tree changed)
 * - 0 if the key was a duplicate (tree unchanged)
 * - -1 on allocation failure (tree unchanged)
 *
 * Required behavior:
 * - Insert as in BST (using tree->nil as leaves), color new node RED.
 * - Fix-up until all RB invariants hold.
 * - Ensure the root is BLACK.
 * - size increases only when a new node is created.
 */
int rbt_insert(RBTree *tree, int key);

/*
 * Remove a key from the tree.
 *
 * Returns:
 * - 1 if the key was found and removed
 * - 0 if the key was not present (tree unchanged)
 *
 * Required behavior:
 * - Standard RB delete with transplant and fix-up.
 * - If a BLACK node is removed, run delete-fixup to restore invariants.
 * - Ensure the root is BLACK at the end.
 */
int rbt_remove(RBTree *tree, int key);

/*
 * In-order traversal (ascending keys).
 * The visitor is called once per non-NIL node.
 * If visitor is NULL, traversal does nothing.
 */
void rbt_in_order(const RBTree *tree,
	 void (*visit)(RBTNode *node, void *ctx), void *ctx);

/*
 * Pre-order traversal.
 * Useful for serialization or copying structure.
 * If visitor is NULL, traversal does nothing.
 */
void rbt_pre_order(const RBTree *tree,
	 void (*visit)(RBTNode *node, void *ctx), void *ctx);

/*
 * Post-order traversal.
 * Useful for freeing nodes bottom-up (excluding NIL).
 * If visitor is NULL, traversal does nothing.
 */
void rbt_post_order(const RBTree *tree,
	 void (*visit)(RBTNode *node, void *ctx), void *ctx);

/*
 * Optional helper: find the minimum key in the tree.
 *
 * Returns:
 * - 1 if the tree is non-empty (writes result to *out_key)
 * - 0 if the tree is empty (out_key unchanged)
 */
int rbt_min(const RBTree *tree, int *out_key);

/*
 * Optional helper: find the maximum key in the tree.
 *
 * Returns:
 * - 1 if the tree is non-empty (writes result to *out_key)
 * - 0 if the tree is empty (out_key unchanged)
 */
int rbt_max(const RBTree *tree, int *out_key);

/*
 * Optional helper: validate RB and BST invariants.
 *
 * Returns:
 * - 1 if all invariants hold
 * - 0 otherwise
 *
 * Suggested checks:
 * - Root black, NIL black
 * - No red node with red child
 * - BST ordering
 * - Equal black-height on all root->leaf paths
 */
int rbt_validate(const RBTree *tree);

#endif
