/* Write a function setbits(x,p,n,y) that returns x with the n
bits that begin at position p set to the rightmost n bits of y, leaving the other
bits unchanged. 
https://stackoverflow.com/questions/15698619/what-function-is-kr-exercise-2-6-really-asking-for
https://stackoverflow.com/questions/1415854/kr-c-exercise-help
https://clc-wiki.net/wiki/K&R2_solutions:Chapter_2:Exercise_6 */

#include <stdio.h>

unsigned getbits(unsigned x, int p, int n);
unsigned setbits(unsigned x, int p, int n, unsigned y);

int main()
{
    return 0;
}

/* From the book
 get n bits from position p */
unsigned getbits(unsigned x, int p, int n)
{
    /* Bits are numbered right to left, but retrieved left to right
    assume:
    x = 44
    p = 4
    n = 3
    unsigned is 4 bytes, then x in binary would be
    00000000 00000000 00000000 00101100
    
    p+1-n gives us the offset of the bitset from the rightmost bit
    so (p + 1 - n) == (4 + 1 - 3) == 2
    so shifting x to the right by 2 gives
    00000000 00000000 00000000 00001011 (11 in decimal)
    
    0 in binary is
    00000000 0000000 0000000 00000000
    performing ~ on it gives
    11111111 1111111 1111111 11111111
    shifting this to the left by n (3) gives
    11111111 1111111 1111111 11111000
    then performing ~ on this (so ~(~0 << 3)) gives
    00000000 00000000 0000000 00000111 which is our bitmask (7 in decimal)
    
    (x >> (p + 1 - n)) & ~(~0 << n) ==

    (44 >> (2)) & (7) ==

    11 & 7 ==

    00000000 00000000 00000000 00001011 &
    00000000 00000000 00000000 00000111 ==
    00000000 00000000 00000000 00000011 == 3 in decimal*/
    return (x >> (p + 1 - n)) & ~(~0 << n);
}

/* Using */
unsigned setbits(unsigned x, int p, int n, unsigned y)
{

}