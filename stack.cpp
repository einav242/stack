#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.hpp"
#include <mutex>
#include <unistd.h>
#include <string>
#include "malloc.hpp"

void pop(mystack *s)
{
    if ((*s).top == NULL)
    {
        printf("\nEMPTY STACK\n");
        (*s).size = 0;
        return;
    }
    Node *temp = (*s).top;
    (*s).top = (*s).top->next;
    s->size--;
    new_free(temp);
    printf("Node is out\n\n");
}
void push(mystack *s, char *value)
{
    Node *temp;
    temp = (struct Node *)new_malloc(sizeof(Node));
    for (int i = 0; i < strlen(value); i++)
    {
        temp->data[i] = value[i];
    }
    if ((*s).top == NULL)
    {
        temp->next = NULL;
    }
    else
    {
        temp->next = (*s).top;
    }
    s->size++;
    (*s).top = temp;
    printf("Node is in\n\n");
}

char *Top(mystack *s)
{
    if ((*s).top == NULL)
    {
        return NULL;
    }
    return (*s).top->data;
}
