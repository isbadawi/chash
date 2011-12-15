#ifndef HASHTABLE_9IG3QKD1
#define HASHTABLE_9IG3QKD1

#define HASH_SIZE 1009 /* smallest prime > 1000 */
#define CHASH_GET_AS(type, table, key) ((type*)chash_get(table, key))

/* TODO: Figure out a way not to have this in the header file? */
typedef struct chash_entry {
    char* key;
    void *data;
    struct chash_entry* next;
} chash_entry;

typedef void  (chash_callback_t) (void*);
typedef char* (chash_tostring_t) (void*);

/*
    The main hash table structure. You'd do best to treat this as an opaque
    structure -- you might run into trouble if you mess with the fields
    directly (feel free to read the size field though!).
*/
typedef struct {
    /* The table proper. */
    chash_entry* table[HASH_SIZE]; 
    /* The size of the table. */
    unsigned long size;
    /* This function will be called whenever a value is deleted from the table
       -- a chash returned by chash_new will have this set to a no-op; you can
       then set this directly. A typical value may be free from stdlib.h, or
       a custom function.
    */
    chash_callback_t* free;
} chash;

/*
    Key-value pair -- the return type of chash_items. Useful e.g. for qsort.
*/
typedef struct {
    char* key;
    void *data;
} chash_item;

/* 
   Creates an empty hash table.
*/
chash* chash_new(void);

/*
   Frees the memory taken up by the hash table.
*/
void chash_free(chash* table);

/* 
   Adds the given key-data mapping to the table. 
   If the key already existed in the table, the old data is deleted.
*/
void chash_put(chash* table, char* key, void *data);

/* 
   Returns the data associated with the given key, or NULL if the key does not
   exist in the table. 
*/
void* chash_get(chash* table, char* key);

/*
   Checks if the table contains the given key -- returns 1 if yes, 0 if not.
*/
int chash_contains(chash* table, char* key);

/*
   Removes the mapping with the given key from the hash table.
*/
void chash_del(chash* table, char* key);

/*
   Removes every mapping from the hash_table. Equivalent to calling chash_del
   on every key returned by chash_keys.
*/
void chash_clear(chash* table);

/*
   Updates dest with the mappings from src, overwriting existing mappings.
*/
void chash_update(chash* dest, chash* src);

/*
   Creates a copy of the given table.
*/
chash* chash_copy(chash* table);

/*
   Returns an array of size table->size, containing all the keys in the table
   (in the same order as that returned by chash_values).
*/
char** chash_keys(chash* table);

/*
   Returns an array of size table->size, containing all the value in the table
   (in the same order as that returned by chash_keys).
*/
void** chash_values(chash* table);

/*
   Returns an array of size table->size, containing all the key-value pairs in
   the table (in the same order as that returned by chash_keys and so on.)
*/
chash_item** chash_items(chash* table);

/*
   Returns a string representation of this table. The to_string callback
   is called on each item (that is, with the void pointer), to turn it into
   a string. This string is freed at the end.
*/
char* chash_to_string(chash* table, chash_tostring_t* to_string);

#endif 
