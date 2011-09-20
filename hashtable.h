#ifndef HASHTABLE_9IG3QKD1
#define HASHTABLE_9IG3QKD1

#define HASH_SIZE 1009 /* smallest prime > 1000 */
#define GET_AS(type, table, key) *((type*)get(table, key))

/* TODO: Figure out a way not to have this in the header file? */
typedef struct entry {
    char* key;
    void *data;
    struct entry* next;
} entry;

typedef struct {
    entry* table[HASH_SIZE]; 
} hash_table;

/* 
   Creates an empty hash table.
*/
hash_table* new_hash_table(void);

/*
    Frees the memory taken up by the hash table, but not the data it stores.
*/
void free_hash_table(hash_table* table);

/* 
   Adds the given key-data mapping to the table. If the key already exists,
   the old data is replaced (but not freed -- we do not assume the given
   pointers come from malloc.)
*/
void put(hash_table* table, char* key, void *data);

/* 
   Returns the data associated with the given key, or NULL if the key does not
   exist in the table. 
*/
void* get(hash_table* table, char* key);

/*
   Removes the mapping with the given key from the hash table. If the key
   didn't exist, does nothing.
*/
void del(hash_table* table, char* key);

#endif 
