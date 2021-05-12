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
#define MAX_LINE 100
#define MAXVAL 100 /* maximum depth of val stack */

int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */
int pop_and_print = 1;
int assignment = 0; /* signals if we need to assign to a variable (variables[i]) */
char variable = ' '; /* stores the single variable letter last used, e.g. 'A' */
char line[MAX_LINE]; /* holds one line at a time of input */
unsigned line_index = 0; /* used for processing full line input in getops */
int line_length = 0; /* length of the line, upto but not including the null terminating character */

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
    double v; /* most recently printed value */
    double op2;
    char s[MAXOP];

    double variables[ALPHABET];
    for (int i = 0; i < ALPHABET; i++)
    {
        variables[i] = 0.0;
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
            case VARIABLE:
            {
                /* 3 A =
                should assign 3 to A

                2 A +
                should add 2 and A together and output 5 */
                if (variable >= UPPER_ALPHA_START && variable <= UPPER_ALPHA_END)
                {
                    push(variables[variable - UPPER_ALPHA_START]); /* e.g. if variable and UPPER_ALPHA_START == 0 then it uses 0 as the index */
                }
                else
                {
                    pop_and_print = 0;
                    printf("Error: %s is not in the range %s to %s\n", s, toascii(UPPER_ALPHA_START), toascii(UPPER_ALPHA_END));
                }               
                break;
            }
            case '=':
            {
                pop(); /* disregard the second operand */
                variables[variable - UPPER_ALPHA_START] = pop();
                pop_and_print = 0;
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
            case '\n':
            {
                if (pop_and_print)
                {
                    v = pop();
                    printf("\t%.8g\n", v);
                    break;
                }
                pop_and_print = 1;
                break;
            }
            case '$': /* most recently printed value */
            {
                push(v);
                break;
            }
            case '?': /* print the top of the stack */
            {
                pop_and_print = 0;
                if (sp > 0)
                {
                    v = val[sp - 1];
                    printf("\t%.8g\n", v);
                }
                else
                {
                    printf("Stack is empty\n");
                }
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
                pop_and_print = 0;
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
                pop_and_print = 0;
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
                pop_and_print = 0;
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
            pop_and_print = 0;
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
            pop_and_print = 0;
        }
        return;
    }
    printf("Error: %s is not a valid mathematical function\n", s);
    pop_and_print = 0;
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

/* int getch(void); */
/* void ungetch(int); */
void ungets(char s[]);

/* getop: get next operator or numeric operand
returns the NUMBER symbol if a number has been parsed,
the NAME symbol if a mathematical name has been parsed,
otherwise returns the integer representation of the character read in. */
int getop(char s[])
{
    if (line_index == line_length)
    {
        get_line(s, )
        line_index = 0;
    }

    int i, c;

    while ((s[0] = c = line[line_index++]) == ' ' || c == '\t')
        ;
    s[1] = '\0';
    i = 0;
    if (islower(c)) /* Potentially a mathematical function */
    {
        while (islower(c))
        {
            s[i++] = c;
            c = getch();
        }
        if (c != EOF)
        {
            ungetch(c); /* Read one too many characters so put it back in the buffer */
        }
        s[i] = '\0';
        return NAME;
    }
    if (isupper(c)) /* Variable */
    {
        variable = c;
        return VARIABLE;
    }
    if (!isdigit(c) && c != '.')
    {
        return c; /* not a number */
    }
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

int buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */

/*
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}
*/

/*
void ungetch(int c)/
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
*/

/* It makes sense to use and build upon what we already have so I've used ungetch. */
void ungets(char s[])
{
    size_t i = strlen(s);
    while(i > 0)
    {
        ungetch(s[--i]);
    }
}