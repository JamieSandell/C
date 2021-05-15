/* Write a recursive version of the function reverse(s), which
reverse the string in place. 

void reverse(char s[])
{
    int c, i, j;
    for(i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
} */

#include "stdio.h"
#include <string.h>

void reverse(char s[]);

int main()
{
    char name[] = {"Jamie"};

    printf("%s reversed: ", name);
    reverse(name);
    printf("%s\n", name);

    return 0;
}

void reverse(char s[])
{
    static int i = 0;
    static int j = 0;
    int c;

    if (j == 0)
    {
        j = strlen(s) - 1;
    }

/* for(i = 0, j = strlen(s) - 1; i < j; i++, j--) */
    if ( i < j)
    {
        c = s[i];
        s[i++] = s[j];
        s[j--] = c;
        reverse(s);
    }
}