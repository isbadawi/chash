/* 
   A simple RPN evaluator. Uses a hash table as a dispatcher. Does not do
   much in the way of error checking, so... ya. Could very well segfault
   on invalid input, since it'll try to pop an empty stack.
*/
#include"../../chash.h"
#include"stack.h"
#include<stdio.h>
#include<string.h>
#include<math.h>

typedef int (op_t) (int, int);

int add(int x, int y) { return x + y; }

int sub(int x, int y) { return x - y; }

int mul(int x, int y) { return x * y; }

int div(int x, int y) { return x / y; }

int mod(int x, int y) { return x % y; }

int int_pow(int x, int y) { return pow(x, y); }

void strip_newline(char* s)
{
    int len = strlen(s);
    if (s[len - 1] == '\n')
        s[len - 1] = '\0';
}

int main(void)
{
    chash* table = chash_new();
    chash_put(table, "+", add);
    chash_put(table, "-", sub);
    chash_put(table, "*", mul);
    chash_put(table, "/", div);
    chash_put(table, "%", mod);
    chash_put(table, "^", int_pow);
    printf("Enter RPN expressions involving ints. Allowable operators are"
           "+, -, *, /, %%, and ^. Press Ctrl-C to quit.\n");
    while (1)
    {
        stack* operands = new_stack();
        printf(">>> ");
        char line[100];
        fgets(line, 100, stdin);
        strip_newline(line);
        char* token = strtok(line, " \t\n");
        while (token)
        {
            op_t* op = CHASH_GET_AS(op_t, table, token);
            if (op)
            {
                int y = pop(operands);
                int x = pop(operands);
                push(operands, op(x, y));
            }
            else
            {
                push(operands, atoi(token));
            }
            token = strtok(NULL, " \t\n");
        }
        if (operands->size == 1)
            printf("%d\n", pop(operands));
        else
            printf("Error evaluating expression.\n");
        free_stack(operands);
    }
}
