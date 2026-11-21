#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUF_SIZE 256

//      Actions stuff

typedef int (*funcPtr)(double, double, double *);

typedef struct {
    const char *symbol;
    funcPtr func;
} Action;

int add(double a, double b, double *res)
{
    *res = a + b;
    return EXIT_SUCCESS;
}
int sub(double a, double b, double *res)
{
    *res = a - b;
    return EXIT_SUCCESS;
}
int mul(double a, double b, double *res)
{
    *res = a * b;
    return EXIT_SUCCESS;
}

int dvd(double a, double b, double *res)
{
    if(b == 0){
        printf("ERR: dvd: Can't divide by 0");
        return EXIT_FAILURE;
    }
    *res = a / b;
    return EXIT_SUCCESS;
}

const Action actionsArr[] = {
    {"+", add},
    {"-", sub},
    {"*", mul},
    {"/", dvd}
};

int findAction(char *symbol)
{
    int length = sizeof(actionsArr)/sizeof(Action);
    for(int i = 0; i < length; ++i)
    {
        if(strcmp(actionsArr[i].symbol, symbol) == 0)
            return i;
    }
    return -1;
}

//          Stack
typedef struct Node{
    double val;
    struct Node *prev;
} Node;

void init(Node **top)
{
    *top = NULL;
}

bool is_empty(Node **top)
{
    return *top == NULL;
}

int push(Node **top, double num)
{
    Node *node = malloc( sizeof(Node) );
    if(node == NULL)
    {
        printf("ERR: push: Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    node->prev = *top;
    node->val = num;
    *top = node;
    return EXIT_SUCCESS;
}

int pop(Node **top, double *num)
{
    if(is_empty(top))
    {
        printf("ERR: pop: Stack is empty\n");
        return EXIT_FAILURE;
    }

    Node *tmp_node = *top;
    *num = (*top)->val;
    *top = (*top)->prev;
    free(tmp_node);

    return EXIT_SUCCESS;
}

void printStack(Node **top)
{
    double num = 0;
    while(*top != NULL)
    {
        pop(top, &num);
        printf("%.2f ", num);
    }
    return;
}

int main(void)
{
    Node *top;
    init(&top);

    char str[BUF_SIZE];
    printf("Enter the expression: ");
    fgets(str, BUF_SIZE, stdin);
    str[strcspn(str, "\n")] = '\0';

    char *myPtr = strtok(str, " ");

    while(myPtr != NULL)
    {
        char *endPtr;
        if(strcmp(myPtr, "neg") == 0)
        {
            double a;
            
            if(pop(&top, &a) == EXIT_FAILURE || push(&top, -a) == EXIT_FAILURE)
                return EXIT_FAILURE;
        }else
        {
            int action_id = findAction(myPtr);
            if(action_id == -1)
            {
                double num = strtod(myPtr, &endPtr);
                if(*endPtr != '\0')
                {
                    printf("ERR: main: Unknown operation \'%s\'\n", endPtr);
                    return EXIT_FAILURE;
                }
                if(push(&top, num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            } else{
                double a, b;
                if(pop(&top, &a) == EXIT_FAILURE || pop(&top, &b) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                
                double num;
                if(actionsArr[action_id].func(b, a, &num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                if(push(&top, num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            }

        }

        myPtr = strtok(NULL, " ");
    }

    double num;
    if(pop(&top, &num) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(is_empty(&top))
        printf("Res: %f\n", num);
    else
    {
        push(&top, num);
        printf("Your expression doesn\'t have single answer because of lack of operations.\n");
        printf("data that's left in stack:\n");
        printStack(&top);
    }

    return EXIT_SUCCESS;
}