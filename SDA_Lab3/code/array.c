#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 64
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
        printf("ERR: dvd: Can't divide by 0;\n");
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
    int lenght = sizeof(actionsArr)/sizeof(Action);
    for(int i = 0; i < lenght; ++i)
    {
        if(strcmp(actionsArr[i].symbol, symbol) == 0)
            return i;
    }
    return -1;
}

//      Stack

typedef struct {
    double data[MAX_SIZE];
    int top;
} Stack;


void init(Stack *s)
{
    s->top = -1;
}

bool is_full(Stack *s)
{
    return s->top == MAX_SIZE-1;
}

bool is_empty(Stack *s)
{
    return s->top == -1;
}


int push(Stack *s, double num)
{
    if(is_full(s))
        {
            printf("Error: push: the stack is full.\n");
            return EXIT_FAILURE;
        }
    else
    {
        s->data[++s->top] = num;
        return EXIT_SUCCESS;
    }
}

int pop(Stack *s, double *num)
{
    if(is_empty(s))
    {
        printf("ERR: pop: Stack is empty\n");
        return EXIT_FAILURE;
    }

        *num = s->data[s->top--];
        return EXIT_SUCCESS;
}

void printStack(Stack *s)
{
    for(int i = s->top; i >= 0; --i)
    {
        printf("%.2f ", s->data[i]);
    }
    printf("\n");
    return;
}

int main(void)
{
    Stack s;
    init(&s);

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
            
            if(pop(&s, &a) == EXIT_FAILURE || push(&s, -a) == EXIT_FAILURE)
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
                if(push(&s, num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            } else{
                double a, b;
                if(pop(&s, &a) == EXIT_FAILURE || pop(&s, &b) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                
                double num;
                if(actionsArr[action_id].func(b, a, &num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                if(push(&s, num) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            }

        }

        myPtr = strtok(NULL, " ");
    }

    double num;
    if(pop(&s, &num) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(is_empty(&s))
        printf("Res: %f\n", num);
    else
    {
        push(&s, num);
        printf("Your expression doesn\'t have single answer because of lack of operations.\n");
        printf("data that's left in stack:\n");
        printStack(&s);
    }

    return EXIT_SUCCESS;
}