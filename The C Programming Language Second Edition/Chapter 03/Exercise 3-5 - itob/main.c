/* Write the function itob(n,s,b) that converts the interger n
into a base b character representation in the string s. In particular,
itob(n,s,16) formats n as a hexadecimal integer in s. */

#include <stdio.h>
#include <string.h>

const int MAX_LEN = 32; /* int is 32 bits on my PC */
const int MIN_BASE = 2;
const int MAX_BASE = 36; /* Number of alphanumeric characters available */
const char DIGITS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void itob(int n, char s[], int b);
void reverse(char s[]);

int main()
{
    char buffer[10];
    int i;
    
    for ( i = 2; i <= 20; ++i )
    {
        itob(255, buffer, i);
        printf("Decimal 255 in base %i : %s\n", i, buffer);
    }

    return 0;
}

void itob(int n, char s[], int b)
{
    if (b < MIN_BASE || b > MAX_BASE)
    {
        printf("b must be between %i and %i inclusive.\n", MIN_BASE, MAX_BASE);
        return;
    }

    int sign = (n < 0) ? -1 : 1;
    int i = 0;
    do
    {
        printf("%i\n", DIGITS[n % b]);
        s[i++] = sign * DIGITS[n % b]; /* Get the digits in reverse/right to left */
    } while ((n /= b) > 0); /* Delete the current digit based on the base */
    if (sign < 0)
    {
        s[i++] = '-'; /* We had a negative number, so add the minus sign back */
    }
    s[i] = '\0'; /* Null terminate the string */
    reverse(s); /* Get the number the right way round */
}

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