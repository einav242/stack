#include <stdio.h>

typedef struct block {
    size_t size;
    struct block* next;
} block;

static block list_head={0,0};
static const size_t above = sizeof(size_t);
static const size_t a = 16;
void* new_malloc(size_t size);
void new_free(void* ptr);
void * new_calloc(size_t quantity, size_t size);