
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include"stack.hpp"
#include <mutex>
#include <unistd.h>

std::mutex Mutex_push;
std::mutex Mutex_pop;
std::mutex Mutex_top;
 
void pop(mystack *s) {
    Mutex_pop.lock();
    if ((*s).top == NULL) {
        printf("\nEMPTY STACK\n");
        (*s).size=0;
        return;
    }
        struct Node *temp = (*s).top;
        (*s).top = (*s).top->next; 
        s->size--;
        free(temp); //deleting the node
    Mutex_pop.unlock();
}
void push(mystack *s,char* value) {
    Mutex_push.lock();
    struct Node *temp;
    temp = (struct Node *)malloc(sizeof(struct Node));
    for(int i=0;i<strlen(value);i++)
    {
        temp->data[i]=value[i];
    }
    if ((*s).top == NULL) {
        temp->next = NULL;
    } else {
        temp->next = (*s).top;
        s->size++; // Make the node as top
    }
    (*s).top = temp; // top always points to the newly created node
    printf("Node is Inserted\n\n");
    Mutex_push.unlock();
}

void Top(mystack *s) {
    Mutex_top.lock();
    // Display the elements of the stack
    if ((*s).top == NULL) {
        printf("\nEMPTY STACK\n");
    } else {
        printf("OUTPUT: ");
        for(int i=0;i<strlen((*s).top->data);i++)
        {
            printf("%c",(*s).top->data[i]);
        }
        printf("\n");
    }
    Mutex_top.unlock();
}
