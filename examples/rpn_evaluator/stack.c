#include"stack.h"
#include<stdlib.h>

stack* new_stack(void)
{
    stack* s = (stack*)malloc(sizeof(stack));
    s->head = NULL;
    s->size = 0;
}

static void free_node(node* s)
{
    while (s != NULL)
    {
        node* temp = s->next;
        free(s);
        s = temp;
    }
}

void free_stack(stack* s)
{
    free_node(s->head);
}

void push(stack* s, int value)
{
    node* new_node = (node*)malloc(sizeof(node));
    new_node->next = NULL;
    new_node->value = value;
    if (s->head == NULL)
        s->head = new_node;
    else
    {
        new_node->next = s->head;
        s->head = new_node;
    }
    s->size++;
}

int pop(stack* s)
{
    int ret = s->head->value;
    s->head = s->head->next;
    s->size--;
    return ret;
}
