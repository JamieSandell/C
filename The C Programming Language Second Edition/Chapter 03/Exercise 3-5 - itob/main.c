/* Write the function itob(n,s,b) that converts the interger n
into a base b character representation in the string s. In particular,
itob(n,s,16) formats n as a hexadecimal integer in s. */

#include <stdio.h>
#include <string.h>

const int MAX_LEN = 32;
const int

void itob(int n, char s[], b);
void reverse(char s[]);

int main()
{
    return 0;
}

void 

/* Reverse s in place */
void reverse(char s[])
{
    int i, j, c;
    for(i = 0, j = strlen(s) - 1; j > i; --j)
    {
        c = s[i];
        s[i++] = s[j];
        s[j] = c;
    }
}