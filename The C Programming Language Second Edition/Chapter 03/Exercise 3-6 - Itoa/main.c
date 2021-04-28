/* Write a version of itoa that accepts three arguments instead of
two. The third argument is a minimum field width; the converted number must
be padded with blanks on the left if necessary to make it wide enough. */

#include <stdio.h>
#include <string.h>

void itoa(int n, char s[], unsigned minimum_field_width);
void reverse(char s[]);

int main()
{
    char s[1000];
    int number = -128;
    itoa(number, s, 8);
    printf("Converted %i to a string: %s\n", number, s);
    return 0;
}

void itoa(int n, char s[], unsigned minimum_field_width)
{
    int i = 0;
    int sign = (n < 0) ? -1 : 1;
    int digit_count = 0;

    do
    {
        s[i++] = sign * (n % 10) + '0';
        digit_count++;
    } while ((n /= 10) != 0);
    if (sign < 0)
    {
        s[i++] = '-';
        digit_count++;
    }
    s[i] = '\0';    
    reverse(s);

    /* Insert blanks */
    int number_of_blanks_to_insert = digit_count - minimum_field_width;
    if (number_of_blanks_to_insert > 0)
    {
        int s_length = strlen(s) - 1;
        /* shift all characters over to the right by number_of_blanks_to_insert */
    }
}

/* From page 62.
Reverse string s in place. */
void reverse(char s[])
{
    int c, i, j;
    for(i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}