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

void array_contains(char* str, char** array, int size) {
    while (size--) {
        if (!strcmp(array[size], str)) {
            return;
        }
    }
    cl_fail("chash_keys is missing a key");
}

void test_chashing__get_all_keys(void) {
    int x = 4;
    char y = 'a';
    chash_put(table, "x", &x);
    chash_put(table, "y", &y);
    chash_put(table, "test", "test");
    char** keys = chash_keys(table);
    array_contains("x", keys, 3);
    array_contains("y", keys, 3);
    array_contains("test", keys, 3);
}

void test_chashing__get_all_keys_and_values(void) {
    int x = 4;
    int y = 5;
    int test = 6;
    chash_put(table, "x", &x);
    chash_put(table, "y", &y);
    chash_put(table, "test", &test);
    char** keys = chash_keys(table);    
    void** values = chash_values(table);
    int i;
    for (i = 0; i < table->size; ++i) {
        int expected_val = *((int*)values[i]);
        int actual_val = *CHASH_GET_AS(int, table, keys[i]);
        cl_assert_(expected_val == actual_val, "chash_keys and chash_values don't return items in the same order");
    }
}

void test_chashing__get_items(void) {
    int x = 4;
    int y = 5;
    int test = 6;
    chash_put(table, "x", &x);
    chash_put(table, "y", &y);
    chash_put(table, "test", &test);
    char** keys = chash_keys(table);    
    void** values = chash_values(table);
    chash_item** items = chash_items(table);
    int i; 
    for (i = 0; i < table->size; ++i) {
        cl_assert_(!strcmp(items[i]->key, keys[i]), "chash_items returns keys in wrong order");
        cl_assert_(values[i] == items[i]->data, "chash_items returns values in wrong order");
    }
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
    chash_item** original_items = chash_items(table);
    chash_item** copied_items = chash_items(copy);
    int i;
    for (i = 0; i < table->size; ++i) {
        cl_assert_(!strcmp(original_items[i]->key, copied_items[i]->key) &&
                   original_items[i]->data == copied_items[i]->data,
                   "chash_copy doesn't preserve items");
    }
}
