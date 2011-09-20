/* Hash table, mapping strings to void pointers. */
#include"hashtable.h"
#include<stdlib.h>
#include<string.h>

#define HASH_SIZE 1009 /* smallest prime > 1000 */

int hash(char *str)
{
    unsigned long hash= 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_SIZE;
}

typedef struct entry {
    char* key;
    void *data;
    struct entry* next;
} entry;

entry* new_entry(char* key, void *data)
{
    entry* new_entry = (entry*)malloc(sizeof(entry));
    new_entry->key = (char*)malloc(strlen(key) + 1);
    strcpy(new_entry->key, key);
    new_entry->data = data;
    new_entry->next = NULL; 
    return new_entry;
}

hash_table* new_hash_table(void)
{
    hash_table* table = (hash_table*)malloc(sizeof(hash_table));
    int i;
    for (i = 0; i < HASH_SIZE; ++i)
        table->table[i] = NULL;
    return table;
}

void put(hash_table* table, char* key, void* data)
{
    int hashed_key = hash(key);
    entry* head = table->table[key];
    if (head == NULL)
        table->table[key] = new_entry(key, data);
    else
    {
        int exists = 0;
        entry* i;
        for (i = head; i != NULL; i = i->next)
            if (!(strcmp(i->key, key)))
            {
                exists = 1;
                i->data = data;
                break;
            }
        if (!exists)
        {
            entry* e = new_entry(key, data);
            e->next = head;
            table->table[key] = e;
        }
    }
}

void* get(hash_table* table, char* key)
{
    entry* head;
    for (head = table->table[hash(key)]; head != NULL; head = head->next)
        if (!strcmp(head->key, key))
            return head->data;
    return NULL;
}

void pretty_print(hash_table* table)
{
}
