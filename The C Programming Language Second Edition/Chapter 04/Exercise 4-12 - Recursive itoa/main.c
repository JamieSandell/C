/* Adapt the ideas of printd to write a recursive version of itoa;
that is, convert an integer into a string by calling a recursive routine.

void printd(int n)
{
    if (n < 0)
    {
        putchar('-');
        n = -n;
    }
    if (n / 10)
    {
        printd(n / 10);
    }
    putchar(n % 10 + '0');
}

void itoa(int n, char s[])
{
    int i = 0;
    int sign = (n < 0) ? -1 : 1;

    do
    {
        s[i++] = sign * (n % 10) + '0';
    } while ((n /= 10) != 0);
    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);    
}

 */

#include <stdio.h>
#include <string.h>

void itoa(int n, char s[]);
void reverse(char s[]);

int main()
{
    int number = 42;
    char number_text[100];

    itoa(number, number_text);
    printf("%s\n", number_text);

    return 0;
}

void itoa(int n, char s[])
{
    static int i;
    static int sign = 0;

    if (sign == 0) /* first call of the function/not recursed yet */
    {
        sign = (n < 0) ? -1 : 1;
    }

    s[i++] = sign * (n % 10) + '0';
    if ((n /= 10) != 0)
    {
        itoa(n, s);
    }
    else
    {
        if (sign < 0)
        {
            s[i++] = '-';
        }
        s[i] = '\0';
        reverse(s);
    }    
}

void reverse(char s[])
{
    int c, i, j;
    for(i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}