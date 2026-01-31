#include <assert.h>
#include <stdio.h>

#include "hash_table.h"

static void expect_find(HashTable *ht, int key, int expected_found, int expected_value) {
	int value = 0;
	int found = hash_table_find(ht, key, &value);
	assert(found == expected_found);
	if (expected_found) {
		assert(value == expected_value);
	}
}

static void test_basic(void) {
	HashTable ht;
	assert(hash_table_init(&ht, HASH_TABLE_DEFAULT_CAPACITY) == 0);
	assert(hash_table_is_empty(&ht));

	assert(hash_table_insert(&ht, 1, 10) == 0);
	assert(hash_table_insert(&ht, 2, 20) == 0);
	assert(hash_table_size(&ht) == 2);

	expect_find(&ht, 1, 1, 10);
	expect_find(&ht, 3, 0, 0);

	assert(hash_table_insert(&ht, 1, 100) == 1);
	assert(hash_table_size(&ht) == 2);
	expect_find(&ht, 1, 1, 100);

	hash_table_destroy(&ht);
}

static void test_collision_and_deleted_probe(void) {
	HashTable ht;
	assert(hash_table_init(&ht, 8) == 0);

	assert(hash_table_insert(&ht, 0, 10) == 0);
	assert(hash_table_insert(&ht, 8, 20) == 0);
	assert(hash_table_insert(&ht, 16, 30) == 0);

	assert(hash_table_remove(&ht, 0) == 1);
	expect_find(&ht, 16, 1, 30);
	expect_find(&ht, 0, 0, 0);

	hash_table_destroy(&ht);
}

static void test_deleted_reuse_no_empty(void) {
	HashTable ht;
	assert(hash_table_init(&ht, 1) == 0);

	assert(hash_table_insert(&ht, 5, 50) == 0);
	assert(hash_table_remove(&ht, 5) == 1);

	assert(hash_table_insert(&ht, 7, 70) == 0);
	expect_find(&ht, 7, 1, 70);

	hash_table_destroy(&ht);
}

static void test_resize_with_deleted(void) {
	HashTable ht;
	assert(hash_table_init(&ht, 8) == 0);

	for (int i = 0; i < 6; i++) {
		assert(hash_table_insert(&ht, i, i * 10) == 0);
	}
	assert(hash_table_remove(&ht, 1) == 1);
	assert(hash_table_remove(&ht, 3) == 1);

	assert(hash_table_insert(&ht, 8, 80) == 0);
	assert(hash_table_insert(&ht, 9, 90) == 0);
	assert(hash_table_insert(&ht, 10, 100) == 0);

	expect_find(&ht, 1, 0, 0);
	expect_find(&ht, 3, 0, 0);
	expect_find(&ht, 0, 1, 0);
	expect_find(&ht, 2, 1, 20);
	expect_find(&ht, 4, 1, 40);
	expect_find(&ht, 5, 1, 50);
	expect_find(&ht, 8, 1, 80);
	expect_find(&ht, 9, 1, 90);
	expect_find(&ht, 10, 1, 100);

	hash_table_destroy(&ht);
}

int main(void) {
	test_basic();
	test_collision_and_deleted_probe();
	test_deleted_reuse_no_empty();
	test_resize_with_deleted();
	printf("All tests passed\n");
	return 0;
}
