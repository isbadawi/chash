#include<stdio.h>
#include<string.h>
#include "clar.h"
#include "chash.h"

chash* table;
int items_deleted;

void increment_delete_count(void *data) {
    items_deleted++;
}

void test_chashing__initialize(void) {
    items_deleted = 0;
    table = chash_new();
    table->free = increment_delete_count;
}

void test_chashing__cleanup(void) {
    chash_free(table);
}

void test_chashing__new_chash(void) {
    cl_assert_(table != NULL, "chash_new returns NULL");
    cl_assert_(table->size == 0, "chash_new returns table with non-zero size");
}

void test_chashing__get_missing_key(void) {
    void *data = chash_get(table, "nosuchkey");
    cl_assert_(data == NULL, "chash_get returns non-NULL for missing key");
}

void test_chashing__get_existing_key(void) {
    int x = 4;
    chash_put(table, "x", &x);
    int data = *CHASH_GET_AS(int, table, "x");
    cl_assert_(data == 4, "chash_get returns wrong data for key");
}

void test_chashing__put_same_key_twice(void) {
    int x1 = 4;
    int x2 = 5;
    chash_put(table, "x", &x1);
    chash_put(table, "x", &x2);
    cl_assert_(items_deleted == 1, "chash_put does not delete old data");
    int data = *CHASH_GET_AS(int, table, "x");
    cl_assert_(data == 5, "chash_get returns wrong data for duplicate key");
}

void test_chashing__delete_key(void) {
    int x = 4;
    chash_put(table, "x", &x);
    chash_del(table, "x");
    cl_assert_(items_deleted == 1, "chash_del doesn't delete data");
    cl_assert_(!chash_contains_key(table, "x"), "chash_del doesn't remove mapping");
}

void test_chashing__contains(void) {
    cl_assert_(!chash_contains_key(table, "x"), "chash_contains_key returns 1 for missing key");
    cl_assert_(!chash_contains_value(table, "somestring"), "chash_contains_value returns 1 for missing value");
    chash_put(table, "x", "somestring");
    cl_assert_(chash_contains_key(table, "x"), "chash_contains_key returns 0 for existing key");
    cl_assert_(chash_contains_value(table, "somestring"), "chash_contains_value returns 0 for existing value");
}

void test_chashing__put_put_del_put(void) {
    chash_put(table, "x", "test");
    chash_put(table, "x", "test2");
    chash_del(table, "x");
    chash_put(table, "x", "newval");
    cl_assert_(items_deleted == 2, "put_put_del_put should entail 2 deletes");
}

void test_chashing__clear(void) {
    chash_put(table, "x", "test");
    chash_put(table, "y", "test");
    chash_put(table, "z", "test");
    chash_clear(table);
    cl_assert_(items_deleted == 3, "chash_clear doesn't remove every item");
    cl_assert_(table->size == 0, "chash_clear doesn't leave the table empty");
}

void test_chashing__copy(void) {
    chash_put(table, "x", "test1");
    chash_put(table, "y", "test2");
    chash_put(table, "z", "test3");
    chash* copy = chash_copy(table);
    cl_assert_(copy->size == table->size, "chash_copy doesn't preserve size");

    chash_iterator orig_iter;
    chash_iterator_init(&orig_iter, table);
    chash_iterator copy_iter;
    chash_iterator_init(&copy_iter, copy);
    char *orig_key;
    char *copy_key;
    void *orig_value;
    void *copy_value;
    while (chash_iterator_next(&orig_iter, &orig_key, &orig_value) &&
           chash_iterator_next(&copy_iter, &copy_key, &copy_value)) {
        cl_assert_(!strcmp(orig_key, copy_key) &&
                   orig_value == copy_value,
                   "chash_copy doesn't preserve items");
    }
}
