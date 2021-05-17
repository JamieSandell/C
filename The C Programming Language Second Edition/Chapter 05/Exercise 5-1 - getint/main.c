/* As written, getint treats a + or - not followed by a digit as
valid representation of zero. Fix it to push such a character back on the input. */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

int getch();
int getint(int *pn);
void ungetch(int c);

int main()
{
    int input;
    int *p = &input;
    int return_value = getint(p);

    switch(return_value)
    {
        case 0:
        {
            printf("Error: Not a number.\n");
        } break;
        case '-':
        case '+':
        {
            printf("Error: Not a number after - or +\n");
        } break;
        default:
        {
            printf("int from input[0]: is %i\n", input);
        } break;
    }

    return 0;
}

#define BUFFER_SIZE 100
int buffer[BUFFER_SIZE]; /* buffer for getch/ungetch */
int buffer_pointer = 0; /* next free position in the buffer */

int getch()
{
    return (buffer_pointer > 0) ? buffer[--buffer_pointer] : getchar();
}

/* getint: get next integer from input into *pn */
int getint(int *pn)
{
    int c, sign;

    while(isspace(c = getch())) /* skip white space */
        ;
    if (!isdigit(c) && c != EOF && c != '+' && c != '-')
    {
        ungetch(c); /* it's not a number */
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')
    {
        int d = c; /* remember the sign character */
        c = getch();
        if (!isdigit(c))
        {
            if (c != EOF)
            {
                /* next character wasn't a digit, and wasn't EOF so push back the sign character and then the character just read in back on the buffer */
                ungetch(d);                
            }
            ungetch(c);
            return(d); /* to indiciate what happened */            
        }
    }
    for (*pn = 0; isdigit(c); c = getch())
    {
        *pn = 10 * *pn + (c - '0');
    }
    *pn *= sign;
    if (c != EOF)
    {
        ungetch(c);
    }
    return c;
}

void ungetch(int c)
{
    if (buffer_pointer >= BUFFER_SIZE)
    {
        printf("Error: ungetch: too many characters\n");
    }
    else
    {
        buffer[buffer_pointer++] = c;
    }
}