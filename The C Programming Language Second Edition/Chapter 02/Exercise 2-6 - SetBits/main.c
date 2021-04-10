/* Write a function setbits(x,p,n,y) that returns x with the n
bits that begin at position p set to the rightmost n bits of y, leaving the other
bits unchanged. */

#include <stdio.h>

unsigned getbits(unsigned x, int p, int n);
unsigned setbits(unsigned x, int p, int n, unsigned y);

int main()
{
    printf("%u\n",setbits(44, 4, 3, 22));
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
    y = 22 */

    unsigned shifting_distance = p + 1 - n; /* Used to shift bits in to position, in this case << 2 */
    unsigned bitmask = (1 << n) - 1; /* 00000111  as we want to get the last 3 (n) bits of 00010110 (==22 == y)*/
    unsigned digits_of_y = (y & bitmask) << shifting_distance; /* 00010110 &
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