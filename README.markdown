chash
=====

`chash` is a simple C implementation of a hash table. It permits only
C strings as keys, and values are void pointers.

Using chash
-----------

Start by including `chash.h`:

    #include<chash.h>

Each hash table is an instance of `chash`. Create one with `chash_new`:

    chash* mytable = chash_new();

Insert elements with `chash_put`:

    chash_put(mytable, "x", something);

Access them with `chash_get`:

    chash_get(mytable, "x");

Check the numbers of elements by reading the `size` field:

    int size = mytable->size;

Remove them with `chash_del`:

    chash_del(mytable, "x");

And free a table with `chash_free`:

    chash_free(mytable);

A `chash` doesn't assume anything about the data stored within it. In
particular, it won't `free` a pointer if it's removed (either through
`chash_del`, `chash_free` or a duplicate key in `chash_put`). Instead, you
can specify a function to call, which is set by default to a no-op. 
The function must have the same signature as `free`.

    #include<stdlib.h>
    ...
    mytable->free = free;

See the `chash.h` file for a full list of the functions and what they do.
