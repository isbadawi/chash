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

unsigned long chash_size(chash* table) {
  return table->size;
}

int chash_contains_key(chash* table, char *key) {
    return chash_get(table, key) != NULL;
}

int chash_contains_value(chash* table, void *data) {
    chash_iterator iter;
    chash_iterator_init(&iter, table);
    char* key;
    void* value;
    while (chash_iterator_next(&iter, &key, &value)) {
        if (value == data) {
            return 1;
        }
    }
    return 0;
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
    chash_iterator iter;
    chash_iterator_init(&iter, table);
    char* key;
    void* value;
    while (chash_iterator_next(&iter, &key, &value)) {
      chash_del(table, key);
    }
}

void chash_update(chash* dest, chash* src) {
    chash_iterator iter;
    chash_iterator_init(&iter, src);
    char* key;
    void* value;
    while (chash_iterator_next(&iter, &key, &value)) {
      chash_put(dest, key, value);
    }
}

chash* chash_copy(chash *table) {
    chash* result = chash_new();
    if (result == NULL) {
        return NULL;
    }
    chash_update(result, table);
    return result;
}
 
void chash_iterator_init(chash_iterator* iterator, chash* table) {
  iterator->table = table;
  iterator->i = -1;
  iterator->entry = NULL;
}

int chash_iterator_next(chash_iterator* iterator, char** key, void **data) {
  while (iterator->entry == NULL) {
    iterator->i++;
    if (iterator->i == HASH_SIZE) {
      return 0;
    }
    iterator->entry = iterator->table->table[iterator->i]->next;
  }
  *key = iterator->entry->key;
  *data = iterator->entry->data;
  iterator->entry = iterator->entry->next;
  return 1;
}

void chash_dump(chash* table) {
    chash_iterator iter;
    chash_iterator_init(&iter, table);
    char* key;
    void *value;
    while (chash_iterator_next(&iter, &key, &value)) {
      fprintf(stderr, "%s: %p\n", key, value);
    }
}
