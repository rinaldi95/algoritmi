#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/*
 * Hash table with open addressing (linear probing) for int keys and int values.
 *
 * This interface exposes a hash table implemented as a flat array of slots.
 * Collisions are resolved by probing for another slot (default: linear probing).
 * The table grows (rehash) when the load factor exceeds a threshold.
 *
 * Invariants to keep in mind while implementing:
 * - size is the number of OCCUPIED slots (stored key/value pairs).
 * - capacity is the number of slots allocated.
 * - Each slot is in exactly one state: EMPTY, OCCUPIED, or DELETED.
 * - Each key appears at most once (policy: insert updates existing key).
 * - load factor = size / capacity, used to decide when to rehash.
 *
 * Hashing of negative keys:
 * - index = key % capacity; if index < 0, index += capacity.
 *
 * NOTE: All functions assume the pointer to HashTable is non-NULL.
 */

#define HASH_TABLE_DEFAULT_CAPACITY 8
#define HASH_TABLE_DEFAULT_MAX_LOAD 0.70

typedef enum {
	HASH_SLOT_EMPTY = 0,
	HASH_SLOT_OCCUPIED = 1,
	HASH_SLOT_DELETED = 2
} HashSlotState;

typedef struct {
	int key;
	int value;
	HashSlotState state;
} HashEntry;

typedef struct {
	HashEntry *entries; /* Array of slots */
	int capacity;       /* Number of slots */
	int size;           /* Number of OCCUPIED slots */
} HashTable;

/*
 * Initialize an empty hash table with a given initial capacity.
 *
 * - initial_capacity must be >= 1.
 * - On success, returns 0 and sets size = 0.
 * - On failure (invalid capacity or allocation failure), returns non-zero
 *   and leaves the table in a safe, empty state (entries = NULL, size = 0).
 */
int hash_table_init(HashTable *ht, int initial_capacity);

/*
 * Free the slot array, then reset to an empty state.
 *
 * After this call, ht->entries is NULL and size/capacity are 0.
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
 * Return the number of slots currently allocated.
 */
int hash_table_capacity(const HashTable *ht);

/*
 * Insert or update a key/value pair.
 *
 * - If the key is not present, a new slot is used (rehash if needed).
 * - If the key is present, its value is updated.
 * - Returns 0 if a new key was inserted.
 * - Returns 1 if an existing key was updated.
 * - Returns -1 on failure (allocation failure or table full).
 *
 * NOTE:
 * - Probing must treat DELETED as reusable for insertion.
 * - Probing must continue past DELETED for search.
 */
int hash_table_insert(HashTable *ht, int key, int value);

/*
 * Find the value associated with a key.
 *
 * - If found, writes the value to *out_value and returns 1.
 * - If not found, returns 0 and leaves out_value unchanged.
 *
 * NOTE: Probing stops at an EMPTY slot or after capacity probes.
 */
int hash_table_find(const HashTable *ht, int key, int *out_value);

/*
 * Remove the pair with the given key.
 *
 * - If found, marks the slot as DELETED and returns 1.
 * - If not found, returns 0.
 */
int hash_table_remove(HashTable *ht, int key);

#endif
