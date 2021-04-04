/* Write a program to determine the ranges of char, short, int,
and long variables, both signed and unsigned, by printing appropriate
values from standard headers and by direct computation. Harder if you
compute them: determine the ranges of the various floating-point types. */

#include "float.h"
#include "limits.h"
#include "stdio.h"

int main()
{
    printf("Unsigned char range: 0 - %u\n", UCHAR_MAX);
    printf("Signed char range: %d - %d\n", SCHAR_MIN, SCHAR_MAX);
    printf("Unsigned short range: 0 - %u\n", USHRT_MAX);
    printf("Signed short range: %d - %d\n", SHRT_MIN, SHRT_MAX);
    printf("Unsigned int range: 0 - %u\n", UINT_MAX);
    printf("Signed int range: %d - %d\n", INT_MIN, INT_MAX);
    printf("Unsigned long range: 0 - %lu\n", ULONG_MAX);
    printf("Signed long range: %ld - %ld\n", LONG_MIN, LONG_MAX);
    printf("Float range: %e - %e\n", FLT_MIN, FLT_MAX);
    printf("Double range: %e - %e\n", DBL_MIN, DBL_MAX);
    printf("Long double range: %e - %e\n", LDBL_MIN, LDBL_MAX);
    return 0;
}