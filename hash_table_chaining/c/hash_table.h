#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/*
 * Hash table with chaining (separate chaining) for int keys and int values.
 *
 * This interface exposes a hash table implemented as an array of bucket lists.
 * The table grows (rehash) when the load factor exceeds a threshold.
 *
 * Invariants to keep in mind while implementing:
 * - size is the number of key/value pairs currently stored.
 * - capacity is the number of buckets allocated.
 * - buckets[i] points to the head of a singly-linked list (or NULL if empty).
 * - Each key appears at most once (policy: insert updates existing key).
 * - load factor = size / capacity, used to decide when to rehash.
 *
 * Hashing of negative keys:
 * - index = key % capacity; if index < 0, index += capacity.
 *
 * NOTE: All functions assume the pointer to HashTable is non-NULL.
 */

#define HASH_TABLE_DEFAULT_CAPACITY 8
#define HASH_TABLE_DEFAULT_MAX_LOAD 0.75

typedef struct HashNode {
	int key;
	int value;
	struct HashNode *next;
} HashNode;

typedef struct {
	HashNode **buckets; /* Array of bucket heads */
	int capacity;       /* Number of buckets */
	int size;           /* Number of stored pairs */
} HashTable;

/*
 * Initialize an empty hash table with a given initial capacity.
 *
 * - initial_capacity must be >= 1.
 * - On success, returns 0 and sets size = 0.
 * - On failure (invalid capacity or allocation failure), returns non-zero
 *   and leaves the table in a safe, empty state (buckets = NULL, size = 0).
 */
int hash_table_init(HashTable *ht, int initial_capacity);

/*
 * Free all nodes and the bucket array, then reset to an empty state.
 *
 * After this call, ht->buckets is NULL and size/capacity are 0.
 * Safe to call on an already-empty table.
 */
void hash_table_destroy(HashTable *ht);

/*
 * Return 1 if the table is empty, 0 otherwise.
 */
int hash_table_is_empty(const HashTable *ht);

/*
 * Return the number of pairs currently stored in the table.
 */
int hash_table_size(const HashTable *ht);

/*
 * Return the number of buckets currently allocated.
 */
int hash_table_capacity(const HashTable *ht);

/*
 * Insert or update a key/value pair.
 *
 * - If the key is not present, a new node is added (rehash if needed).
 * - If the key is present, its value is updated.
 * - Returns 0 if a new key was inserted.
 * - Returns 1 if an existing key was updated.
 * - Returns -1 on allocation failure (node allocation or rehash).
 */
int hash_table_insert(HashTable *ht, int key, int value);

/*
 * Find the value associated with a key.
 *
 * - If found, writes the value to *out_value and returns 1.
 * - If not found, returns 0 and leaves out_value unchanged.
 */
int hash_table_find(const HashTable *ht, int key, int *out_value);

/*
 * Remove the pair with the given key.
 *
 * - If found, removes the node and returns 1.
 * - If not found, returns 0.
 */
int hash_table_remove(HashTable *ht, int key);

#endif
