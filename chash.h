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
    An iterator over a hash table.
*/
typedef struct {
  chash* table;
  int i;
  chash_entry* entry;
} chash_iterator;

/* 
   Creates an empty hash table. Returns NULL if it couldn't be created
   (i.e. because memory could not be allocated for it.)
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
int chash_contains_key(chash* table, char* key);

/*
   Checks if the table contains the given value -- returns 1 if yes, 0 if not.
*/
int chash_contains_value(chash* table, void* data);

/*
   Removes the mapping with the given key from the hash table.
*/
void chash_del(chash* table, char* key);

/*
   Removes every mapping from the hash_table.
*/
void chash_clear(chash* table);

/*
   Updates dest with the mappings from src, overwriting existing mappings.
*/
void chash_update(chash* dest, chash* src);

/*
   Creates a copy of the given table.
   Returns NULL if it couldn't be created.
*/
chash* chash_copy(chash* table);

/*
   Dumps the contents of the given table (keys and value addresses)
   to stderr. Useful for debugging.
*/
void chash_dump(chash* table);

/*
   Initializes the provided iterator, associating it with the table.
*/
void chash_iterator_init(chash_iterator*, chash* table);

/*
   Advances the given iterator, retrieving the key and value that are pointed
   to after the advancement.

   Returns 0 if there are no more elements, or 1 otherwise.
   If 0 is returned, key and data are not set, and the iterator is
   no longer valid. (It may be reinitialized by another call to
   chash_iterator_init).

   Note that calling chash_del on the retrieved key is allowed, and will
   not invalidate the iterator.
*/
int chash_iterator_next(chash_iterator* iterator, char** key, void **data);

#endif 
