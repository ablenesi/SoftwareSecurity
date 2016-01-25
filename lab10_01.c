#include "pthread.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_THREAD_NUM 1000

typedef struct _MY_LIST_ENTRY
{
    struct _MY_LIST_ENTRY* next;
    struct _MY_LIST_ENTRY* prev;
} MY_LIST_ENTRY, *PMY_LIST_ENTRY;

typedef struct _ELEMENT
{
    MY_LIST_ENTRY Entry;
    int thread_num;
} ELEMENT, *PELEMENT;

MY_LIST_ENTRY   gList;

void
init_list(PMY_LIST_ENTRY list)
{
    list->next = list->prev = list;
}

void
insert_list(PMY_LIST_ENTRY list, PMY_LIST_ENTRY entry)
{
    entry->prev = list;
    entry->next = list->next;
    list->next->prev = entry;
    list->next = entry;
}

int
is_empty_list(PMY_LIST_ENTRY list)
{
    return (list->next == list->prev) && (list->next == list);
}

PMY_LIST_ENTRY
pop_list(PMY_LIST_ENTRY list)
{
    PMY_LIST_ENTRY entry = NULL;

    if (is_empty_list(list))
    {
        return NULL;
    }

    entry = list->prev;
    entry->prev->next = list;
    list->prev = entry->prev;

    return entry;
}

void*
__cdecl
concurrent_function(void *ctx)
{
    int thread_num = (int)ctx;
    PELEMENT entry = NULL;

    entry = malloc(sizeof(ELEMENT));
    if (NULL == entry)
    {
        goto cleanup;
    }

    entry->thread_num = thread_num;

    insert_list(&gList, &entry->Entry);
    entry = (PELEMENT)pop_list(&gList);

cleanup:
    if (NULL != entry)
    {
        free(entry);
    }

    pthread_exit(0);

    return NULL;
}

int main()
{
    pthread_t tids[MAX_THREAD_NUM];
    int i;
    int j;

    init_list(&gList);

    for (j = 0; j < 1000; j++)
    {
        for (i = 0; i < MAX_THREAD_NUM; i++)
        {
            pthread_create(&tids[i], NULL, concurrent_function, (void*)i);
        }

        for (i = 0; i < MAX_THREAD_NUM; i++)
        {
            pthread_join(tids[i], NULL);
        }
        printf(".");
    }
}