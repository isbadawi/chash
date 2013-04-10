/* Hash table, mapping strings to void pointers. */
#include"chash.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

static int hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_SIZE;
}

static void default_free(void* p) {
    return;
}

static chash_entry* new_chash_entry(char* key, void *data) {
    chash_entry* new_chash_entry = malloc(sizeof(*new_chash_entry));
    if (new_chash_entry == NULL) {
        return NULL;
    }
    new_chash_entry->key = malloc(strlen(key) + 1);
    if (new_chash_entry->key == NULL) {
        free(new_chash_entry);
        return NULL;
    }
    strcpy(new_chash_entry->key, key);
    new_chash_entry->data = data;
    new_chash_entry->next = NULL; 
    return new_chash_entry;
}

static void free_chash_entry(chash_entry* e, chash_callback_t* on_free) {
    if (e == NULL) {
        return;
    }
    free_chash_entry(e->next, on_free);
    free(e->key);
    on_free(e->data);
    free(e);
}
  
chash* chash_new(void) {
    chash* table = malloc(sizeof(*table));
    if (table == NULL) {
        return NULL;
    }
    int i;
    for (i = 0; i < HASH_SIZE; ++i) {
        table->table[i] = malloc(sizeof(*(table->table[i])));
        if (table->table[i] == NULL) {
            int j;
            for (j = 0; j < i; ++j) {
                free(table->table[j]);
            }
            free(table);
            return NULL;
        }
        table->table[i]->key = NULL;
        table->table[i]->data = NULL;
        table->table[i]->next = NULL;
    }
    table->size = 0;
    table->free = default_free;
    return table;
}

void chash_free(chash* table) {
    int i;
    for (i = 0; i < HASH_SIZE; ++i) {
        free_chash_entry(table->table[i]->next, table->free);
        free(table->table[i]);
    }
    free(table);
}

void chash_put(chash* table, char* key, void* data) {
    int hashed_key = hash(key);
    chash_entry* head = table->table[hashed_key];
    chash_entry* i;
    for (i = head->next; i != NULL; i = i->next) {
        if (!strcmp(i->key, key)) {
            void *temp = i->data;
            i->data = data;
            table->free(temp);
            return;
        }
    }
    chash_entry* e = new_chash_entry(key, data);
    e->next = head->next;
    head->next = e;
    table->size += 1;
}

void* chash_get(chash* table, char* key) {
    chash_entry* head = table->table[hash(key)]->next;
    for (; head != NULL; head = head->next) {
        if (!strcmp(head->key, key)) {
            return head->data;
        }
    }
    return NULL;
}

int chash_contains_key(chash* table, char *key) {
    return chash_get(table, key) != NULL;
}

int chash_contains_value(chash* table, void *data) {
    void** values = chash_values(table);
    int i;
    for (i = 0; i < table->size; ++i) {
        if (values[i] == data) {
            break;
        }
    }
    free(values);
    return i < table->size;
}

void chash_del(chash* table, char* key) {
    int hashed_key = hash(key);
    chash_entry* head = table->table[hashed_key];
    chash_entry* prev = head;
    for (head = head->next; head != NULL; head = head->next) {
        if (!strcmp(head->key, key)) {
            prev->next = head->next;
            free(head->key);
            table->free(head->data);
            free(head);
            table->size -= 1;
            break;
        }
        prev = head;
    }
}

void chash_clear(chash* table) {
    char **keys = chash_keys(table);
    int i;
    /* chash_del modifies table->size */
    int size = table->size;
    for (i = 0; i < size; ++i) {
        chash_del(table, keys[i]);
    }
    free(keys);
}

void chash_update(chash* dest, chash* src) {
    chash_item **items = chash_items(src);
    int i;
    for (i = 0; i < src->size; ++i) {
        chash_put(dest, items[i]->key, items[i]->data);
        free(items[i]);
    }
    free(items);
}

chash* chash_copy(chash *table)
{
    chash* result = chash_new();
    if (result == NULL) {
        return NULL;
    }
    chash_update(result, table);
    return result;
}

char** chash_keys(chash* table) {
    char** keys = malloc(table->size * sizeof(*keys));
    if (keys == NULL) {
        return NULL;
    }
    int key = 0;
    int i;
    chash_entry* head;
    for (i = 0; i < HASH_SIZE; ++i) {
        for (head = table->table[i]->next; head != NULL; head = head->next) {
            keys[key++] = head->key;
        }
    }
    return keys;
}

void** chash_values(chash* table) {
    void** values = malloc(table->size * sizeof(*values));
    if (values == NULL) {
        return NULL;
    }
    int value = 0;
    int i;
    for (i = 0; i < HASH_SIZE; ++i) {
        chash_entry* head;
        for (head = table->table[i]->next; head != NULL; head = head->next) {
            values[value++] = head->data;
        }
    }
    return values;
}      

chash_item** chash_items(chash* table) {
    chash_item** items = malloc(table->size * sizeof(*items));
    if (items == NULL) {
        return NULL;
    }
    int item = 0;
    int i;
    for (i = 0; i < HASH_SIZE; ++i) {
        chash_entry* head;
        for (head = table->table[i]->next; head != NULL; head = head->next) {
            items[item] = malloc(sizeof(*(items[item])));
            if (items[item] == NULL) {
                int j;
                for (j = 0; j < item; ++j) {
                    free(items[j]);
                }
                free(items);
                return NULL;
            }
            items[item]->key = head->key;
            items[item]->data = head->data;
            item++;
        }
    }
    return items;
}

char* chash_to_string(chash* table, chash_tostring_t* to_string) {
    char** keys = chash_keys(table);
    if (keys == NULL) {
        return NULL;
    }
    void** values = chash_values(table);
    if (values == NULL) {
        free(keys);
        return NULL;
    }
    char** strings = malloc(table->size * sizeof(*strings));
    if (strings == NULL) {
        free(keys);
        free(values);
        return NULL;
    }
    int length = 0;
    int i;
    for (i = 0; i < table->size; ++i) {
        strings[i] = to_string(values[i]);
        length += strlen(keys[i]);
        length += strlen(strings[i]);
        length += 6; /* quotes for the key, colon & space separators, comma, space */
    }
    /* -2 for the trailing comma & space, +2 for the braces */

    char *result = malloc(length + 1);
    if (result == NULL) {
        int j;
        for (j = 0; j < table->size; ++j) {
            free(strings[j]);
        }
        free(keys);
        free(values);
        free(strings);
        return NULL;
    }

    strcpy(result, "{");
    int from = 1;
    for (i = 0; i < table->size; ++i) {
        from += sprintf(result + from, "\"%s\": %s", keys[i], strings[i]);    
        if (i != table->size - 1) {
            from += sprintf(result + from, ", ");
        }
        free(strings[i]);
    }
    strcpy(result + from, "}");
    free(keys);
    free(values);
    return result;
}
