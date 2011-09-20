#include<stdio.h>
#include "minunit.h"
#include "chash.h"

int tests_run = 0;

chash* table;

void set_up(void)
{
    table = chash_new();
}

void tear_down(void)
{
    chash_free(table, 0);
}

char* test_new_chash(void)
{
    mu_assert("new_hash_table returns NULL", table != NULL);
    return 0;
}

char* test_get_missing_key(void)
{
    void *data = chash_get(table, "nosuchkey");
    mu_assert("table returns non-NULL data for missing key", data == NULL);
    return 0;
}

char* test_get_existing_key(void)
{
    int x = 4;
    void* exists = chash_put(table, "x", &x);
    mu_assert("chash_put returns non-NULL for new key", exists == NULL);
    int data = CHASH_GET_AS(int, table, "x");
    mu_assert("table returns wrong data for key", data == 4);
    return 0;
}

char* test_put_same_key_twice(void)
{
    int x1 = 4;
    int x2 = 5;
    chash_put(table, "x", &x1);
    void* old = chash_put(table, "x", &x2);
    mu_assert("chash_put does not return old data", *((int*)old) == 4);
    int data = CHASH_GET_AS(int, table, "x");
    mu_assert("table returns wrong data for duplicate key", data == 5);
    return 0;
}

char* test_delete_key(void)
{
    int x = 4;
    chash_put(table, "x", &x);
    void* del = chash_del(table, "x");
    void* data = chash_get(table, "x");
    mu_assert("chash_del does not return deleted data", *((int*)del) == 4);
    mu_assert("table return non-NULL for deleted key", data == NULL);
    return 0;
}