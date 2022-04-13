#define SIZE 1024

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
void Top(mystack *s);