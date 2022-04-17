#include<string>
#define SIZE 1024
using namespace std;

typedef struct Node {
    char data[SIZE];
    struct Node *next;
}Node; 

typedef struct mystack
{
   Node* top;
   int size; 
}mystack;

void pop(mystack *s);
void push(mystack *s,char* value);
char* Top(mystack *s);
void display(mystack *s);