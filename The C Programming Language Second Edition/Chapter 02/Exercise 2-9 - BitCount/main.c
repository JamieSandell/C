/* In a two's complement number system, x &= (x-1) deletes the
rightmost 1-bit in x. Explain why. Use this observation to write a faster version of bitcount.

The bitcount referred to is:
int bitcount(unsigned x)
{
    int b;

    for (b = 0; x != 0; x >>= 1)
    {
        if (x & 01)
        {
            b++;
        }
    }
    return b;
} */

#include <stdio.h>

/* x &= (x - 1) deletes the right most bit in a two's complement system.
This is because to negate a number, each bit of the number is inverted (zeroes are replaced with ones and vice versa).
Then one (000...00001) is added (ignoring overflow). This avoids the two representations for 0 found in one complement by using all ones to represent -1.
000...0010 = +2
000...0001 = +1
111...1111 = -1
111...1110 = -2*/
int bitcount(unsigned x)
{
    int b;

    for (b = 0; x != 0; x &= (x -1 ))
    {
        b++;
    }
    
    return b;
}

int main()
{
    unsigned number = 12;
    printf("bitcounts of %u = %i", number, bitcount(number));
    return 0;
}