/* Write the program expr, which evaluates a reverse Polish
expression from the command line, where each operator or operand is a separate argument. For example,
    expr 2 3 4 + *
evaluates 2 x (3+4). */

/* An alternate organisation uses getline to read an entire input
line; this makes getch and ungetch unnecessary. Revise the calculator to use
this approach */

#include <ctype.h>
#include <math.h> /* for fmod */
#include <stdio.h>
#include <stdlib.h> /* for atof() */
#include <string.h>

#define ALPHABET 26 /* number of letters in the alphabet */
#define UPPER_ALPHA_START 65 /* ASCII table */
#define UPPER_ALPHA_END 90
#define MAXOP 100 /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define NAME '1' /* signal that a mathematical name was found such as sin */
#define VARIABLE '2' /* signal that a variable was found */
#define MAXVAL 100 /* maximum depth of val stack */

int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */
int pop_and_print = 1;
int assignment = 0; /* signals if we need to assign to a variable (variables[i]) */
char variable = ' '; /* stores the single variable letter last used, e.g. 'A' */
char line[MAXOP]; /* holds one line of input at a time */
unsigned line_index = 0;
int line_length = 0; /* length of the line, upto but not including the null terminating character */

void push(double f);
double pop(void);

/* reverse Polish calculator 
 while (next operator or operand is not end-of-file indicate)
    if (number)
        push it
    else if (operator)
        pop operands
        do operation
        push result
    else if (newline)
        pop and print top of stack
    else
        error */
int main(int argc, char *argv[])
{
    int type;
    int c;
    double v; /* most recently printed value */
    double op2;
    char s[MAXOP];

    double variables[ALPHABET];
    for (int i = 0; i < ALPHABET; i++)
    {
        variables[i] = 0.0;
    }

    while(--argc > 0) /* at least more than one argument passed in, as technically the program name counts as 1 argument */
    {
        *++argv; /* point to the next address of full 1D (char) array, as the first one is the program name,
        then dereference it to get the address of the first element in said array. */

        /* **argv dereferences what argv points to (pointer to address of full 1D array), to give the address of the first element of said array,
        and then dereferences that to give the actual value (first element in that 1D array). */
        c = **argv;
        if (!isdigit(c))
        {
            type =  c; /* not a number */
        }
        else
        {
            type = NUMBER;
        }
        switch (type)
        {
            case NUMBER:
            {
                push(atof(*argv));
                break;
            }
            case '+':
            {
                push(pop() + pop());
                break;
            }
            case '*': /* need to escape this if calling from the shell */
            {
                push(pop() * pop());
                break;
            }
            case '-':
            {
                op2 = pop();
                push(pop() - op2); /* The order in which two pop()s are evaluated is not defined */
                break;
            }
            case '/':
            {
                op2 = pop();
                if (op2 != 0.0)
                {
                    push(pop() / op2);
                }
                else
                {
                    printf("Error: zero divisor\n");
                    pop_and_print = 0;
                }                
                break;
            }
            case '%':
            {
                op2 = pop();
                if (op2 != 0.0)
                {
                    push(fmod(pop(), op2));
                }
                else
                {
                    printf("Error: zero divisor\n");
                    pop_and_print = 0;
                }                
                break;
            }
            case '^':
            {
                if (sp > 1)
                {
                    op2 = pop();
                    push(pow(pop(), op2));
                    break;
                }
                printf("Error: Not enough elements on the stack for ^\n");
                pop_and_print = 0;
                break;
            }
            default:
            {
                printf("Error: unknown command %s\n", s);
                pop_and_print = 0;
                break;
            }
        }
    }

    printf("Final result = %lf\n", pop());
    return 0;
}

/* push: push f onto value stack */
void push(double f)
{
    if (sp < MAXVAL)
    {
        val[sp++] = f;
    }
    else
    {
        printf("Error: stack full, can't push %g\n"), f;
    }
}

/* pop: pop and return top value from stack */
double pop(void)
{
    if (sp > 0)
    {
        return val[--sp];
    }
    else
    {
        printf("Error: stack empty\n");
        return 0.0;
    }
}