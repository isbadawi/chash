#ifndef STACK_XI11CYIE
#define STACK_XI11CYIE

/* An integer stack. */
typedef struct node
{
    int value;
    struct node* next;
} node;

typedef struct stack
{
    node* head;
    int size;
} stack;

stack* new_stack(void);
void free_stack(stack* s);
void push(stack* s, int value);
int pop(stack* s);

#endif 
