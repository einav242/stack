#define SIZE 1024

typedef struct Node {
    char data[1024];
    struct Node *next;
}Node; 

typedef struct mystack
{
   Node* top;
   int size; 

}mystack;

char* pop(mystack *s);
void push(mystack *s,char* value);
void Top(mystack *s);