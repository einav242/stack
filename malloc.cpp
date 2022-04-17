#include <stdio.h>
#include <unistd.h>
#include "malloc.hpp"
#include <string.h>

void *new_malloc(size_t size)
{
    size = (size + sizeof(size_t) + (a - 1)) & ~(a - 1);
    block *block_h = list_head.next;
    block **head = &(list_head.next);
    while (block_h != 0)
    {
        if (block_h->size >= size)
        {
            *head = block_h->next;
            return sizeof(size_t) + ((char*)block_h);
        }
        head = &(block_h->next);
        block_h = block_h->next;
    }
    brk(&size);
    block_h = (block*)sbrk(size);
    block_h->size = size;

    return sizeof(size_t) + ((char*)block_h);
}

void * new_calloc(size_t quantity, size_t size)
{
    void *ptr;
    ptr = new_malloc(size * quantity);
    if (ptr == 0)
    {
        return (ptr);
    }
    memset(ptr,0,size * quantity);
    return (ptr);
}

void new_free(void *ptr)
{
    block *block_h = (block *)(((char *)ptr) - sizeof(size_t));
    block_h->next =list_head.next;
    list_head.next = block_h;
}