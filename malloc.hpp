#include <stdio.h>

typedef struct free_block {
    size_t size;
    struct free_block* next;
} free_block;

static free_block free_block_list_head = { 0, 0 };
static const size_t overhead = sizeof(size_t);
static const size_t align_to = 16;
void* new_malloc(size_t size);
void new_free(void* ptr);