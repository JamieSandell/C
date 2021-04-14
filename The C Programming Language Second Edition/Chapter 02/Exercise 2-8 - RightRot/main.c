/* Write a function rightrot(x,n) that returns the value of the integer x rotated to the right by n bit positions. */

#include <stdio.h>

#define SIZE_OF_INT 32 /* In bits */

unsigned getbits(unsigned x, int p, int n); /* From the previous exercise */
unsigned rightrot(unsigned x, int n);

int main()
{
    unsigned x = 127;
    int n = 3;
    printf("%u rotated to the right by %i bit positions = %u\n", x, n, rightrot(x, n));
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

/* Doesn't stop you going over the width of an integer, so no clamping of n within the function.
Right circular shift (moving the first bit to the last position while shifting all other bits to the previous position.
E.g. x = 127, n = 3, code assumes ints are 32 bits.
00000000 00000000 00000000 01111111 becomes
11100000 00000000 00000000 00001111 */
unsigned rightrot(unsigned x, int n)
{
    /* grab the n bits of x */
    unsigned n_bits_of_x = getbits(x, n - 1, n);   /* 00000000 00000000 00000000 00000111 */
    unsigned x_shifted_right = x >> n;             /* 00000000 00000000 00000000 00001111 */
    unsigned shift_left = SIZE_OF_INT - 1 - n;
    unsigned bit_mask = n_bits_of_x << shift_left; /* 11100000 00000000 00000000 00000000 */
    return bit_mask | x_shifted_right;             /* 11100000 00000000 00000000 00000000 |
                                                      00000000 00000000 00000000 00000111 ==
                                                      11100000 00000000 00000000 00001111 == decimal 1879048207 */
}