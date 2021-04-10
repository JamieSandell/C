/* Write a function invert(x,p,n) that returns x with the n bits
that begin at position p inverted (i.e., 1 changed into 0 and vice versa), leaving
the others unchanged. */

#include <stdio.h>

unsigned getbits(unsigned x, int p, int n);
unsigned invert(unsigned x, int p, int n);
unsigned setbits(unsigned x, int p, int n, unsigned y); /* From the previous exercise */

int main()
{
    unsigned x = 42;
    int p = 4;
    int n = 3;

    printf("Inverted %i bits at position %i of %u: %u (decimal)\n", n, p, x, invert(x, p, n));

    return 0;
}

/* From the book
 get n bits from position p */
unsigned getbits(unsigned x, int p, int n)
{
    /* assume:
    x = 44
    p = 4
    n = 3
    unsigned is 4 bytes, then x in binary would be
    00000000 00000000 00000000 00101100
    so here are the positions of the last byte:
    7 6 5 4 3 2 1 0
    0 0 1 0 1 1 0 0
    so in this instance getbits should return the bits at position (p = 4) 4 and the number of bits 3 (n = 3),
    so it should return 011, and we can drop the leading 0.
    
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
    00000000 00000000 00000000 00000011 == (3 in decimal) which is exactly the bits we wanted to return. */
    return (x >> (p + 1 - n)) & ~(~0 << n);
}

/* Assume:
x = 42 == 00101010
p = 4
n = 3
shifting_distance ==  2

so bits to invert are ---010--
so the result of inverting ---010-- == ---101-- == 00110110 (54 decimal)
*/
unsigned invert(unsigned x, int p, int n)
{
    unsigned shifting_distance = p + 1 - n;
    unsigned bitmask = ~(~0 << n); /* 00000111 */
    unsigned x_bits = getbits(x, p, n); /* 00000010 */
    x_bits = x_bits ^ bitmask; /* 00000010 ^
                                  00000111 ==
                                  00000101 */


    return setbits(x, p, n, x_bits);
}

/* Write a function setbits(x,p,n,y) that returns x with the n
bits that begin at position p set to the rightmost n bits of y, leaving the other
bits unchanged.
assume:
x = 44
p = 4
n = 3
y = 22

binary representations of x and y
0000000 0000000 00000000 00101100 (44) x
0000000 0000000 00000000 00010110 (22) y
0000000 0000000 00000000 00000110 are the bits to insert from y in to x, if p = 4 and n = 3
0000000 0000000 00000000 00111000 should be the result (56) */
unsigned setbits(unsigned x, int p, int n, unsigned y)
{
    /* Where relevant the comments assume:
    x = 44
    p = 4
    n = 3
    y = 22
    
    What we want is to get the last 3 (n) bits of y, which is 00000110, then shift it in to position (p + 1 - n), so it becomes:
    00111000, so --111--- is what we need to insert in to x, but we need to leave the left of 111 (--) and the right of 111 (---) the same.
    To do that we create a bitmask, so those 3 (n) bits of y at the right hand side are turned on (1s) and everything else turned off (0s),
    which would be 00000111, then we shift our bitmask in to place (p + 1 - n, or << 2), so it becomes
    00011100, and then we invert it so it's 11100011, then we AND it with x and store it in x, so x is now:
    00100000.
    Then we add the bits of y we shifted in to position earlier (---110--) with x, we do this by ORing x and those y numbers.
    The result being 00100000 | 00011000 == 00111000 (56 decimal)
     */

    unsigned shifting_distance = p + 1 - n; /* Used to shift bits in to position, in this case << 2 */
    unsigned bitmask = (1 << n) - 1; /* 00000111  as we want to get the last 3 (n) bits of 00010110 (==22 == y)*/
    unsigned digits_of_y = getbits(y, n - 1, n) << shifting_distance; /* 00010110 &
                                                                         00000111 << 2 ==
                                                                         00000110 << 2 == 
                                                                         00011000, so this puts the 3 digits from y into position (p = 4)*/
    unsigned bitmask_shifted_to_the_left = bitmask << shifting_distance; /* 00000111 << 2 == 00011100 */
    unsigned inverted_bitmask_shifted_to_the_left = ~bitmask_shifted_to_the_left; /* ~00011100 == 11100011 */
    /* Erase the middle bits of x */
    x = x & inverted_bitmask_shifted_to_the_left; /* 00101100 &
                                                     11100011 ==
                                                     00100000 */
    /* Add those bits from y into x */
    x = x | digits_of_y; /* 00100000 |
                            00011000 ==
                            00111000 == 56 in decimal*/
    return x;    
}