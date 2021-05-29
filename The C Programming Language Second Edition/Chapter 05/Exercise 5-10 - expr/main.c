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

int get_line(char s[], unsigned max_line_length);
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
        *++argv; /* point to the next char array, as the first one is the program name */
        /* get the argument type */
        if (!isdigit(c = **argv) && strlen(*argv) == 1) /* e.g. c = argv[1][0]
        {
            type = c
        }
        switch (type)
        {
            case NUMBER:
            {
                push(atof(*argv));
                break;
            }
            case NAME:
            {
                math_function(*argv);
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

int get_line(char s[], unsigned max_line_length)
{
    int c;
    int i = 0;
    while((c = getchar()) && c < max_line_length && c != '\n' && c != EOF)
    {
        s[i++] = c;
    }
    if (c == '\n' || c == EOF)
    {
        s[i++] = c;
    }
    s[i] = '\0';
    return i;
}

/* getop: get next operator or numeric operand
returns the NUMBER symbol if a number has been parsed,
the NAME symbol if a mathematical name has been parsed,
otherwise returns the integer representation of the character read in. */
int getop(char s[])
{
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
            c = line[line_index++];
        }
        if (c != EOF)
        {
            line_index--; /* Read one too many characters so put it back in the buffer */
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
        if (c == '\n')
        {
            line_index = line_length;
        }
        return c; /* not a number */
    }
    if (isdigit(c)) /* collect integer part */
    {
        while (isdigit(s[++i] = c = line[line_index++]))
            ;
    }
    if (c == '.') /* collect fraction part */
    {
        while(isdigit(s[++i] = c = line[line_index++]))
            ;
    }
    s[i] = '\0';
    if (c != EOF)
    {
        line[line_index--];
    }
    return NUMBER;
}

void ungets(char s[])
{
    line_index = 0;
}