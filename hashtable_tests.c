#include<stdio.h>
#include "minunit.h"
#include "hashtable.h"

int tests_run = 0;

char* test_new_hash_table(void)
{
    hash_table* table = new_hash_table();
    mu_assert("new_hash_table returns NULL", table != NULL);
    free_hash_table(table);
    return 0;
}

char* test_get_missing_key(void)
{
    hash_table* table = new_hash_table();
    void *data = get(table, "nosuchkey");
    mu_assert("table returns non-NULL data for missing key", data == NULL);
    free_hash_table(table);
    return 0;
}

char* test_get_existing_key(void)
{
    hash_table* table = new_hash_table();
    int x = 4;
    put(table, "x", &x);
    int data = *((int*)get(table, "x"));
    mu_assert("table returns wrong data for key", data == 4);
    free_hash_table(table);
    return 0;
}

char* test_put_same_key_twice(void)
{
    hash_table* table = new_hash_table();
    int x1 = 4;
    int x2 = 5;
    put(table, "x", &x1);
    put(table, "x", &x2);
    int data = *((int*)get(table, "x"));
    mu_assert("table returns wrong data for duplicate key", data == 5);
    free_hash_table(table);
    return 0;
}

char* run_tests(void)
{
    mu_run_test(test_new_hash_table);
    mu_run_test(test_get_missing_key);
    mu_run_test(test_get_existing_key);
    mu_run_test(test_put_same_key_twice);
    return 0;
}

int main(int argc, const char *argv[])
{
    char* result = run_tests();
    if (result)
        printf("%s\n", result);
    else
        printf("ALL TESTS PASSED\n");
    printf("Tests run: %d\n", tests_run);
    return 0;
}
