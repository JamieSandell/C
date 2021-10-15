/* Rewrite the postfix calculator of Chapter 4 to use scanf and/or sscanf to do the input and number conversion */

#include <stdio.h>

#define MAXOP 100 /* max size of operand or operator */
#define MAXVAL 100 /* maximum depth of val stack */
#define NUMBER '0' /* signal that a number was found */

int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */

void push (double);
double pop(void);

/* reverse Polish calculator */
int main()
{
    char *c;
    char s[MAXOP], buf[MAXOP];
    double a = 0, op2;
    char e = '\0';

    while(scanf("%s%c", s, &e) == 2) /* Read from the standard input. Store the string (number or operand) in s and the char (space) in e. */
    {
        if (sscanf(s, "%lf", &a) == 1) /* Read from the string, is it a number? */
        {
            push(a);
        }
        else if (sscanf(s, "%s", buf) == 1) /* if it wasn't a number, read it into buf */
        {
            for (c = buf; *c; ++c) /* read buf until the null terminator */
            {
                switch (*c)
                {
                    case '+':
                        push(pop() + pop());
                        break;
                    case '-':
                        op2 = pop();
                        push(pop() - op2);
                        break;
                    case '*':
                        push(pop() * pop());
                        break;
                    case '/':
                        op2 = pop();
                        if (op2 != 0.0) /* avoid divide by zero error */
                        {
                            push(pop() / op2);
                        }
                        else
                        {
                            printf("Error: attempt to divide by zero\n");
                        }                        
                        break;
                    default:
                        printf("Error: unknown command\n");
                        break;
                }
            }
            if (e == '\n')
            {
                printf("\t%.8g\n", pop());
            }
        }
    }

    return 0;
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

/* push: push f onto value stack */
void push(double f)
{
    if (sp < MAXVAL)
    {
        val[sp++] = f;
    }
    else
    {
        printf("Error: stack full, can't push %g\n", f);
    }
}