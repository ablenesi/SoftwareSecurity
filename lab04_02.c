#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    wSTRING,
    wINTEGER,
} word_type;

// double linked list
typedef struct _list_entry
{
    struct _list_entry* next;
    struct _list_entry* prev;
} list_entry;

typedef struct
{
    list_entry entry;
    word_type  type;

    union
    {
        char* word;
        long integer;
    };

} word_t;

typedef void(*cbFunc) (list_entry*);

//============================================================================

word_t* getword(list_entry* entry)
{
    return (word_t*)entry;
}

// Set pointers to itself
void list_init(list_entry* list)
{
    list->next = list;
    list->prev = list;
}

// error for writing
void list_push_back(list_entry* list, list_entry* entry)
{
    entry->next = list;
    entry->prev = list->prev;
    list->prev->next=entry;
    list->prev = entry;
}

// Allocate a new word_t
word_t* walloc()
{
    word_t* res;

    res = malloc(sizeof(word_t));
    
    if(res == NULL){
        return NULL;
    }

    list_init(&res->entry);

    return res;
}

// Allocate and set the given long
word_t* walloc_int(long value)
{
    word_t* res;

    res = walloc();
    
    if(res == NULL){
        return NULL;
    }

    res->type = wINTEGER;
    res->integer = value;

    return res;
}

word_t* walloc_char(char* value)
{
    word_t* res;
    res = walloc();

    if(res == NULL){
        return NULL;
    }
    
    res->type = wSTRING;
    res->word = value;

    return res;
}

// Recursive free for the linked list
void wfree(word_t* word)
{
    if(word->type == wSTRING){
        free(word->word);
    }
    free(word);
}

list_entry* list_delete_entry(list_entry* entry)
{
    list_entry* prev = entry->prev;

    entry->prev->next = entry->next;
    entry->next->prev = prev;

    return entry;
}

void list_uninit(list_entry* list)
{
    list_entry* node;

    // while the list is not empty
    while (list->next != list)
    {
        node = list_delete_entry(list->next);
        wfree(getword(node));
    }
}

void list_iterate(list_entry* list, cbFunc callback)
{
    list_entry* entry = list->next;

    while (entry != list)
    {
        callback(entry);
        entry = entry->next;
    }
}

void print_word(list_entry* entry)
{
    word_t* w;

    w = getword(entry);

    if (w->type == wINTEGER)
    {
        printf("%d\n", w->integer);
    }
    else
    {
        printf("%s\n", w->word);
    }
}

void process_word(list_entry* list, char* word)
{
    long intvalue;
    char* wend, my_word = NULL;
    word_t* w = NULL;
    size_t size;

    intvalue = strtol(word, &wend, 10); // string to long

    if (intvalue != 0 || word != wend)
    {
        w = walloc_int(intvalue);
        if (NULL == w) goto _error;
    }
    else
    {
        size = strlen(word);
        my_word = malloc(size + 1);
        if (NULL == my_word)
            goto _error;
        memcpy(my_word, word, size+1);
        w = walloc_char(my_word);
        if (NULL == w)
            goto _error;
    }

    list_push_back(list, &w->entry);
    return;

_error:
    if(w != NULL){
        wfree(w);
    }
    if(my_word!=NULL){
        free(my_word);
    }
}

int main()
{
    char lbuff[100];
    list_entry list;

    list_init(&list);

    do
    {
        scanf("%99s", lbuff);

        if (0 == strcmp("exit", lbuff))
        {
            break;
        }

        process_word(&list, lbuff);
    } while (1);

    list_iterate(&list, print_word);

    list_uninit(&list);
}
