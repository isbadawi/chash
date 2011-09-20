#include<stdio.h>
#include "minunit.h"
#include "hashtable.h"

int tests_run = 0;

char* test_new_hash_table(void)
{
    hash_table* table = new_hash_table();
    mu_assert("error, new_hash_table returns NULL", table != NULL);
    return 0;
}

char* run_tests(void)
{
    mu_run_test(test_new_hash_table);
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
