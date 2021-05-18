/* Write getfloat, the floating-point analog of getint. What
type does getfloat return as its function value? */

#include <math.h>
#include <ctype.h>
#include <stdio.h>

int getch();
int getfloat(double *pn);
void ungetch(int c);

int main()
{
    double input;
    double *p = &input;
    int return_value = getfloat(p);

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
            printf("float from input: is %f\n", input);
        } break;
    }
    return 0;
}

#define BUFFER_SIZE 100
int buffer[BUFFER_SIZE];
int buffer_index = 0; /* points to the next free buffer entry */

int getch()
{
    return (buffer_index > 0) ? buffer[--buffer_index] : getchar();
}

/* getfloat: get next float from input into *pn */
int getfloat(double *pn)
{
    int c, sign;
    double power;

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
    for (*pn = 0.0; isdigit(c); c = getch())
    {
        *pn = 10.0 * *pn + (c - '0');
    }
    if (c == '.') /* get the fractional part */
    {
        c = getch();
    }
    for (power = 1.0; isdigit(c); c = getch())
    {
        *pn = 10.0 * *pn + (c - '0');
        power *= 10.0;
    }
    
    *pn *= sign / power;
    if (c != EOF)
    {
        ungetch(c);
    }
    return c;
}

void ungetch(int c)
{
    if (buffer_index < BUFFER_SIZE)
    {
        buffer[buffer_index++] = c;
    }
    else
    {
        printf("Error: Buffer full\n");
    }
}