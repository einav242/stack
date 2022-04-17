#include <stdio.h>
#include <unistd.h>
#include "malloc.hpp"
#include <strings.h>

void *new_malloc(size_t size)
{
    size = (size + sizeof(size_t) + (align_to - 1)) & ~(align_to - 1);
    free_block *block = free_block_list_head.next;
    free_block **head = &(free_block_list_head.next);
    while (block != 0)
    {
        if (block->size >= size)
        {
            *head = block->next;
            return ((char *)block) + sizeof(size_t);
        }
        head = &(block->next);
        block = block->next;
    }
    brk(&size);
    block = (free_block *)sbrk(size);
    block->size = size;

    return ((char *)block) + sizeof(size_t);
}

void * calloc(size_t nelem, size_t elsize)
{
    void *p;
    p = new_malloc(nelem * elsize);
    if (p == 0)
        return (p);

    bzero(p, nelem * elsize);
    return (p);
}

void new_free(void *ptr)
{
    free_block *block = (free_block *)(((char *)ptr) - sizeof(size_t));
    block->next = free_block_list_head.next;
    free_block_list_head.next = block;
}