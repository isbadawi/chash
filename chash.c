/* Hash table, mapping strings to void pointers. */
#include"chash.h"
#include<stdlib.h>
#include<string.h>

static int hash(char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_SIZE;
}

static entry* new_entry(char* key, void *data)
{
    entry* new_entry = (entry*)malloc(sizeof(entry));
    new_entry->key = (char*)malloc(strlen(key) + 1);
    strcpy(new_entry->key, key);
    new_entry->data = data;
    new_entry->next = NULL; 
    return new_entry;
}

static void free_entry(entry* e, int free_data)
{
    while (e->next != NULL)
    {
        entry* temp = e;
        e = e->next;
        free(temp->key);
        if (free_data)
            free(temp->data);
        free(temp);
    }
}
  
chash* chash_new(void)
{
    chash* table = (chash*)malloc(sizeof(chash));
    int i;
    for (i = 0; i < HASH_SIZE; ++i)
        table->table[i] = NULL;
    table->size = 0;
    return table;
}

void chash_free(chash* table, int free_data)
{
    int i;
    for (i = 0; i < HASH_SIZE; ++i)
        if (table->table[i] != NULL)
            free_entry(table->table[i], free_data);
    free(table);
}

void* chash_put(chash* table, char* key, void* data)
{
    int hashed_key = hash(key);
    entry* head = table->table[hashed_key];
    if (head == NULL)
    {
        table->table[hashed_key] = new_entry(key, data);
        table->size += 1;
    }
    else
    {
        entry* i;
        for (i = head; i != NULL; i = i->next)
            if (!(strcmp(i->key, key)))
            {
                void *temp = i->data;
                i->data = data;
                return temp;
            }
        entry* e = new_entry(key, data);
        e->next = head;
        table->table[hashed_key] = e;
        table->size += 1;
    }
    return NULL;
}

void* chash_get(chash* table, char* key)
{
    entry* head;
    for (head = table->table[hash(key)]; head != NULL; head = head->next)
        if (!strcmp(head->key, key))
            return head->data;
    return NULL;
}

void* chash_del(chash* table, char* key)
{
    int hashed_key = hash(key);
    entry* head = table->table[hashed_key];
    if (head == NULL)
        return;
    if (!strcmp(head->key, key))
    {
        free(head->key);
        table->table[hashed_key] = head->next;
        table->size -= 1;
        void *data = head->data;
        free(head);
        return data;
    }
    entry* prev = head;
    for (head = head->next; head != NULL; head = head->next)
    {
        if (!strcmp(head->key, key))
        {
            free(head->key);
            prev->next = head->next;
            table->size -= 1;
            void *data = head->data;
            free(head);
            return data;
        }
        prev = head;
    }
    return NULL;
}
