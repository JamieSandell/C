/* Add commands for handling variables. (It's easy to provide
twenty-six variables with single-letter names.) Add a variable for the most
recenly value. */

#include <math.h> /* for fmod */
#include <stdio.h>
#include <stdlib.h> /* for atof() */
#include <string.h>

#define ALPHABET 26 /* number of letters in the alphabet */
#define MAXOP 100 /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define NAME '1' /* signale that a mathematial name was found such as sin */
#define MAXVAL 100 /* maximum depth of val stack */

int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */

int getop(char s[]);
void math_function(char s[]);
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
int main()
{
    int type;
    double op2;
    char s[MAXOP];
    int pop_and_print = 1;

    double variable[ALPHABET];
    for (int i = 0; i < ALPHABET; i++)
    {
        variable[i] = 0.0;
    }

    while ((type = getop(s)) != EOF)
    {
        switch (type)
        {
            case NUMBER:
            {
                push(atof(s));
                break;
            }
            case NAME:
            {
                math_function(s);
                break;
            }
            case '+':
            {
                push(pop() + pop());
                break;
            }
            case '*':
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
                break;
            }
            case '\n':
            {
                if (pop_and_print)
                {
                    printf("\t%.8g\n", pop());
                    break;
                }
                pop_and_print = 1;
                break;
            }
            case '?': /* print the top of the stack */
            {
                pop_and_print = 0;
                (sp > 0) ? printf("\t%.8g\n", val[sp - 1]) : printf("Stack is empty\n");
                break;
            }
            case '~': /* swap the top two elements */
            {
                pop_and_print = 0;
                if (sp > 1)
                {
                    double temp = val[sp - 1];
                    val[sp - 1] = val[sp - 2];
                    val[sp - 2] = temp;
                    break;
                }
                printf("Error: Need more than 1 elements on the stack to swap\n");
                break;
            }
            case '#': /* duplicate */
            {
                pop_and_print = 0;
                if (sp < MAXVAL)
                {
                    push(val[sp - 1]);
                    break;
                }
                printf("Error: stack full, can't duplicate\n");
                break;
            }
            case '!': /* clear the stack */
            {
                pop_and_print = 0;
                sp = 0;
                break;
            }
            default:
            {
                printf("Error: unknown command %s\n", s);
                break;
            }
        }
    }

    return 0;
}

/* perform a mathematical function based on s */
void math_function(char s[])
{
    if (strcmp(s, "sin") == 0)
    {
        if (sp > 0)
        {
            push(sin(pop()));
        }
        else
        {
            printf("Error: Not enough elements on the stack for sin\n");
        }
        return;
    }
    if (strcmp(s, "exp") == 0)
    {
        if (sp > 0)
        {
            push(exp(pop()));
        }
        else
        {
            printf("Error: Not enough elements on the stack for exp\n");
        }
        return;
    }
    printf("Error: %s is not a valid mathematical function\n", s);
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

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop: get next operator or numeric operand
returns the NUMBER symbol if a number has been parsed,
the NAME symbol if a mathematical name has been parsed,
otherwise returns -1. */
int getop(char s[])
{
    int i, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';
    if (isalpha)
    {
        return NAME;
    }
    if (!isdigit(c) && c != '.')
    {
        return -1; /* not a number */
    }
    i = 0;
    if (isdigit(c)) /* collect integer part */
    {
        while (isdigit(s[++i] = c = getch()))
            ;
    }
    if (c == '.') /* collect fraction part */
    {
        while(isdigit(s[++i] = c = getch()))
            ;
    }
    s[i] = '\0';
    if (c != EOF)
    {
        ungetch(c);
    }
    return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */

int getch(void) /* get a (possibly pushed back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE)
    {
        printf("ungetch: too many characters\n");
    }
    else
    {
        buf[bufp++] = c;
    }
}