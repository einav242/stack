#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include"stack.hpp"
#include <mutex>
#include <unistd.h>
#include<string>

std::mutex Mutex_push;
std::mutex Mutex_pop;
std::mutex Mutex_top;
 
void pop(mystack *s) {
    Mutex_pop.lock();
    if ((*s).top == NULL) {
        printf("\nEMPTY STACK\n");
        (*s).size=0;
        Mutex_pop.unlock();
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

char*  Top(mystack *s) {
    Mutex_top.lock();
    if ((*s).top == NULL) {
        Mutex_top.unlock();
        char *d=(char*)malloc(21);
        strcpy(d,"OUTPUT: Empty stack");
       return d;
    } 
    char *value=(char*)malloc(strlen((*s).top->data)+8);
    char d[8]="OUTPUT:";
    int j;
    int i;
    for(j=0;j<strlen(d);j++)
    {
        value[j]=d[j];
    }
    for(i=0;i<strlen((*s).top->data);i++)
    {
        value[j]=(*s).top->data[i];
        j++;
    }
    value[j]='\0';
    puts(value);
    Mutex_top.unlock();
    return value;
}
