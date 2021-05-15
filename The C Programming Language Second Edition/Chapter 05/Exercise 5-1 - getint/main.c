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
    char input[] = {"100\n"};
    size_t length = strlen(input);
    int x;

    for (int i = 0; i < length; i++)
    {
        ungetch(input[i]);
    }

    printf("int from input: %i\n", getint(&x));

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
        c = getch();
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