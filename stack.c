
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include"stack.h"
 
char* pop(mystack *s) {
    if ((*s).top == NULL) {
        printf("\nEMPTY STACK\n");
        (*s).size=0;
        return NULL;
    }
        struct Node *temp = (*s).top;
        char temp_data[1024];
        printf("Popped element is :");
        for(int i=0;i<strlen((*s).top->data);i++)
        {
            temp_data[i]=(*s).top->data[i];
            printf("%c",temp_data[i]);
        }
        printf("\n");
        (*s).top = (*s).top->next; 
        s->size--;
        free(temp); //deleting the node
        return temp_data;
}
void push(mystack *s,char* value) {
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
}

void Top(mystack *s) {
    // Display the elements of the stack
    if ((*s).top == NULL) {
        printf("\nEMPTY STACK\n");
    } else {
        printf("The value is \n");
        for(int i=0;i<strlen((*s).top->data);i++)
        {
            printf("%c",(*s).top->data[i]);
        }
        printf("\n");
    }
}
