#include <stdio.h>
#include <unistd.h>
#include "malloc.hpp"
#include"stack.hpp"
#include <string.h>
int current=sizeof(Node);

void* new_malloc (size_t size) {
    void* ans;
    if((ans=sbrk(current))==(void*)(-1))
    {
        perror("sbrk");
        exit(1);
    }
    current += current;
    return ans;
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

void new_free (void* ptr) {
    if(sbrk(current)==(void*)(-1))
    {
        perror("sbrk");
        exit(1);
    }
    current -= sizeof(Node);
}


