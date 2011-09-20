#include<stdio.h>
#include "minunit.h"
#include "hashtable.h"

int tests_run = 0;

hash_table* table;

void set_up(void)
{
    table = new_hash_table();
}

void tear_down(void)
{
    free_hash_table(table);
}

char* test_new_hash_table(void)
{
    mu_assert("new_hash_table returns NULL", table != NULL);
    return 0;
}

char* test_get_missing_key(void)
{
    void *data = get(table, "nosuchkey");
    mu_assert("table returns non-NULL data for missing key", data == NULL);
    return 0;
}

char* test_get_existing_key(void)
{
    int x = 4;
    put(table, "x", &x);
    int data = GET_AS(int, table, "x");
    mu_assert("table returns wrong data for key", data == 4);
    return 0;
}

char* test_put_same_key_twice(void)
{
    int x1 = 4;
    int x2 = 5;
    put(table, "x", &x1);
    put(table, "x", &x2);
    int data = GET_AS(int, table, "x");
    mu_assert("table returns wrong data for duplicate key", data == 5);
    return 0;
}

char* test_delete_key(void)
{
    int x = 4;
    put(table, "x", &x);
    del(table, "x");
    void* data = get(table, "x");
    mu_assert("table return non-NULL for deleted key", data == NULL);
    return 0;
}
