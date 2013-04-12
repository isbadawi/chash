chash
=====

`chash` is a simple C implementation of a hash table. It permits only
C strings as keys, and values are void pointers.

Using chash
-----------

Start by including `chash.h`:

```c
#include<chash.h>
```

Each hash table is an instance of `chash`. Create one with `chash_new`:

```c
chash* mytable = chash_new();
```

Insert elements with `chash_put`:

```c
chash_put(mytable, "x", something);
```

Access them with `chash_get`:

```c
void *something = chash_get(mytable, "x");
```

Check the numbers of elements with `chash_size`:

```c
int size = chash_size(mytable);
```

Remove them with `chash_del`:

```c
chash_del(mytable, "x");
```

Iterate over the entries using a `chash_iterator`:

```c
chash_iterator iter;
chash_iterator_init(&iter, mytable);
char *key; void *value;
while (chash_iterator_next(&iter, &key, &value)) {
    /* Do something with key and value here.
       Note it is safe to call chash_del(mytable, key) here.  */
}
```

Free a table with `chash_free`:

```c
chash_free(mytable);
```

A `chash` doesn't assume anything about the data stored within it. In
particular, it won't `free` a pointer if it's removed (either through
`chash_del`, `chash_free` or a duplicate key in `chash_put`). Instead, you
can specify a function to call, which is set by default to a no-op. 
The function must have the same signature as `free`, e.g.

```c
chash *mytable = chash_new();
mytable->free = free;
```
