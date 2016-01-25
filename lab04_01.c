#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_INIT_SIZE 1024
#define BUFFER_MAX_SIZE  10240

typedef struct {
    size_t size;
    char* buffer;
    size_t pos;
} buffer_t;

typedef struct {
    unsigned long tag;
    size_t size;
    char buffer[0];
} memory_t;

buffer_t g_buffer;

void fatal();

memory_t* getmem(void* mem)
{
    return (memory_t*)(((char*)mem) - sizeof(memory_t));
}

void* myalloc(size_t size, unsigned long tag)
{
    memory_t* res;

    if (size == 0)
        return NULL;

    res = malloc(size + sizeof(memory_t));
    res->tag = tag;
    res->size = size;

    return &res->buffer;
}

void myfree(void* memory, unsigned long tag)
{
    memory_t* res;

    res = getmem(memory);
    if (res->tag != tag)
    {
        fatal();
    }

    free(res);
}

void* myrealloc(void* memory, size_t size)
{
    void* res;
    memory_t* mem;

    mem = getmem(memory);
    res = myalloc(size, mem->tag);
    memcpy(res, memory, mem->size);
    myfree(memory, mem->tag);

    return res;
}

void buffer_init(buffer_t* buffer)
{
    buffer->size = BUFFER_INIT_SIZE;
    buffer->pos = 0;
    buffer->buffer = myalloc(buffer->size, 'BUFF');
}

void buffer_append(buffer_t* buffer, size_t size, char* toappend)
{
    if (buffer->pos + size > buffer->size)
    {
        buffer->size += size;
        if (buffer->size > BUFFER_MAX_SIZE)
        {
            fatal();
        }
        buffer->buffer = myrealloc(buffer->buffer, buffer->size);
    }

    memcpy(&buffer->buffer[buffer->pos], toappend, size);
    buffer->pos += size;
    buffer->buffer[buffer->pos] = 0;
}

void buffer_free(buffer_t* buffer)
{
    myfree(buffer->buffer, 'BUFF');
}

void fatal()
{
    printf("Fatal error occurred\n");

    memset(g_buffer.buffer, 0, g_buffer.size);
    buffer_free(&g_buffer);

    exit(1);
}

int main()
{
    char lbuf[100];
    size_t ret;
    size_t len;

    buffer_init(&g_buffer);
    do
    {
        printf(">");
        ret = scanf("%99s", lbuf);

        if (0 == strcmp("exit", lbuf))
        {
            break;
        }

        len = strlen(lbuf);
        buffer_append(&g_buffer, len, lbuf);
    } while (ret != 0);

    printf("\n==========================\n");
    printf("%s\n", g_buffer.buffer);

    buffer_free(&g_buffer);
}