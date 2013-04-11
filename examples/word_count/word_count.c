/* The standard "read a file and sort its words by frequency" exercise. */
#include"../../chash.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

typedef struct {
  char *key;
  void *value;
} pair;

int compare_by_frequency(const void* p1, const void* p2)
{
    pair pair1 = *((pair*) p1);
    pair pair2 = *((pair*) p2);
    int val1 = *((int*) pair1.value);
    int val2 = *((int*) pair2.value);
    if (val1 < val2)
        return 1;
    else if (val1 == val2)
        return 0;
    return -1;
}

void see_word(chash* table, char* str)
{
    int* val = CHASH_GET_AS(int, table, str);
    if (val)
        (*val)++;
    else
    {
        int* new_int = (int*)malloc(sizeof(int));
        *new_int = 1;
        chash_put(table, str, new_int);
    }
}

/* 
   Get a file's entire contents as a string.
   Note that this will only work for actual files on disk, not e.g. for stdin.
*/
char* read_entire_file(FILE* fp)
{
    struct stat buf;
    fstat(fileno(fp), &buf);
    int file_size = buf.st_size;
    char* contents = malloc(file_size + 1);
    fread(contents, 1, file_size, fp);
    contents[file_size] = '\0';
    return contents;
}

/* Strip punctation, and lower case. */
char* clean_word(char* word)
{
    int len = strlen(word);
    char *new_word = (char*)malloc(len + 1);
    int new_word_index = 0;
    int i;
    for (i = 0; i < len; ++i)
        if (!ispunct(word[i]))
            new_word[new_word_index++] = tolower(word[i]);
    if (new_word_index == 0)
    {
        free(new_word);
        return NULL;
    }
    new_word[new_word_index] = '\0';
    return new_word;
}

int main(int argc, char* argv[])
{
    chash* table = chash_new();
    char* text = read_entire_file(fopen(argv[1], "r"));
    char* token = strtok(text, " \t\n");
    while (token)
    {
        char* cleaned_word = clean_word(token);
        if (cleaned_word != NULL)
        {
            see_word(table, cleaned_word);
            free(cleaned_word);
        }
        token = strtok(NULL, " \t\n");
    }

    chash_iterator iter;
    chash_iterator_init(&iter, table);
    char *key; void *value;
    pair* pairs = malloc(table->size * sizeof(*pairs));
    int i = 0;
    while (chash_iterator_next(&iter, &key, &value)) {
      pairs[i].key = key;
      pairs[i].value = value;
      i++;
    }
    qsort(pairs, table->size, sizeof(pair), compare_by_frequency);
    printf("Words sorted by frequency:\n");
    for (i = 0; i < table->size; ++i)
        printf("%s: %d\n", pairs[i].key, *((int*)pairs[i].value));
    chash_free(table);
    return 0;
}
