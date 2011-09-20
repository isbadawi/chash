#ifndef HASHTABLE_9IG3QKD1
#define HASHTABLE_9IG3QKD1

#define HASH_SIZE 1009 /* smallest prime > 1000 */
#define CHASH_GET_AS(type, table, key) *((type*)chash_get(table, key))

/* TODO: Figure out a way not to have this in the header file? */
typedef struct entry {
    char* key;
    void *data;
    struct entry* next;
} entry;

typedef struct {
    entry* table[HASH_SIZE]; 
} chash;

/* 
   Creates an empty hash table.
*/
chash* chash_new(void);

/*
    Frees the memory taken up by the hash table, but not the data it stores.
*/
void chash_free(chash* table);

/* 
   Adds the given key-data mapping to the table. If the key already exists,
   the old data is replaced (but not freed -- we do not assume the given
   pointers come from malloc.)
*/
void chash_put(chash* table, char* key, void *data);

/* 
   Returns the data associated with the given key, or NULL if the key does not
   exist in the table. 
*/
void* chash_get(chash* table, char* key);

/*
   Removes the mapping with the given key from the hash table, and returns
   the data (freeing it is up to the caller). If the key didn't exist, 
   return NULL.
*/
void* chash_del(chash* table, char* key);

#endif 
