/* In a two's complement number representation, our version of
itoa does not handle the largest negative number, that is, the value of n equal
to -(2 to the power of wordsize-1). Explain why not. Modify it to print that value correctly,
regardless of the machine on which it runs. 

itoa: convert n to characters in s
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0) # records the sign
        n = -n; # make n positive
    i = 0;
    do { # generate digits in reverse order
        s[i++] = n % 10 + '0'; # get next digit
    } while ((n /= 10) > 0); # delete it
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
*/

/* Wordsize is most likely 32 bits, but if we pretend it's 8 bits, then the largest negative number that can be stored is
-(2 to the power of wordsize-1) =
-(2 to the power of 8-1) =
-128 

however the biggest positive number we can store is 2 to the power of n-1 - 1 = 127.
So at  */

#include <stdio.h>

int main()
{
    return 0;
}