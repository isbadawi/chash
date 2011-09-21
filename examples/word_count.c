/* The standard "read a file and sort its words by frequency" exercise. */
#include"chash.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

int compare_by_frequency(const void* p1, const void* p2)
{
    int data1 = *((int*)((*((chash_item**)p1))->data));
    int data2 = *((int*)((*((chash_item**)p2))->data));
    if (data1 < data2)
        return 1;
    else if (data1 == data2)
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
    chash_item** items = chash_items(table);
    qsort(items, table->size, sizeof(chash_item*), compare_by_frequency);
    printf("Words sorted by frequency:\n");
    int i;
    for (i = 0; i < table->size; ++i)
        printf("%s: %d\n", items[i]->key, *((int*)items[i]->data));
    chash_free(table, 1);
    return 0;
}
