/* Write the function htoi(s), which converts a string of hexadecimal
digits (including an optional 0x or 0X) into its equivalent integer value.
The allowable digits are 0 through 9, a through f, and A through F. */
#include <stdio.h>

#define BASE 16

int htoi(char hex[]);
int upper(int c);

int main()
{
    return 0;
}

int htoi(char hex[])
{
    int first = upper(hex[0]);
    int second = upper(hex[1]);
    int pos = 0;
    if (first == '0' && second == 'X') /* Optional 0X used? */
    {
        pos = 2;
    }

    for (int i = pos; i != '\0'; i++)
    {
        
    }
}

/* Convert lower case ASCII to upper case ASCII */
int upper(int c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - 32;
    }
    else
    {
        return c;
    }
}