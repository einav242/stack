#include<stdio.h>
#include"stack.h"
#include<string.h>
#include<stdlib.h>

int main() {
    int choice;
    char value[1024]; 
    mystack *s=(mystack*)malloc(sizeof(mystack));
    printf("\nImplementaion of Stack using Linked List\n");
    while (1) {
        printf("1. Push\n2. Pop\n3. Top\n4. Exit\n");
        printf("\nEnter your choice : ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            printf("\nEnter the value to insert: ");
            memset(value, 0, strlen(value));
            scanf("%s", value);
            push(s,value);
            break;
        case 2:
            pop(s);
            break;
        case 3:
            Top(s);
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("\nWrong Choice\n");
        }
    }

}