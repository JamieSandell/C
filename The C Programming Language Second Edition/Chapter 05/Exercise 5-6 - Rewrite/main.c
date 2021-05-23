/* Rewrite appropriate programs from earlier chapters and exercises 
with pointers instead of array indexing. Good possibilities include getline
(Chapters 1 and 4), atoi, itoa, and their variants (Chapters 2, 3, and 4),
reverse (Chapter 3), and strindex and getop (Chapter 4). */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 1000
#define MAX_LINE_LENGTH 100
#define MAX_SIZE 1000

int get_line(char *s, int lim);

double atof(char *s);
double power(double x, int y);

void itoa(int n, char *s, unsigned minimum_field_width);
void reverse(char *s);

int strrindex(char *source, char *pattern);

int main()
{
    /*
    char line[MAX_LINE_LENGTH];
    char *line_p = line;
    while(get_line(line_p, MAX_LINE_LENGTH) > 0)
    {
        printf("%s\n", line_p);
    }
    */

    /*
    char number[] = {"10e-2"};
    printf("%s converted to double: %f\n", number, atof(number));
    */

    /*
    char s[MAX_SIZE];
    int number = -128;
    itoa(number, s, 8);
    printf("Converted %i to a string: %s\n", number, s);
    */

    /*
    char pattern[] = {"ould"};
    char line[MAX_LINE];
    int found = 0;
    int result;

    while(get_line(line, MAX_LINE) > 0)
    {
        if ((result = strrindex(line, pattern)) >= 0)
        {
            printf("Found %s at index %i of line: %s", pattern, result, line);
            found++;
        }
    }
    */

    return 0;
}

int get_line(char *s, int lim)
{
    int c, i;

    i = 0;
    while(--lim > 0 && (c = getchar()) != EOF && c != '\n')
    {
        *(s + i++) = c;
    }
    if (c == '\n')
    {
        *(s + i++) = c;
    }
    *(s + i) = '\0';

    return i;
}


double atof(char *s)
{
    double val, val_power;
    int i, sign, exponent, exponent_sign;

    for (i = 0; isspace(*(s + i)); i++) /* ignore white space */
        ;
    sign = (*(s + i) == '-') ? -1 : 1; /* store the sign */
    if (*(s + i)== '+' || *(s + i) == '-')
    {
        i++;
    }
    for (val = 0.0; isdigit(*(s + i)); i++) /* convert to the nth column */
    {
        val = 10.0 * val + (*(s + i) - '0');
    }
    if (*(s + i) == '.') /* deal with the decimal */
    {
        i++;
    }
    for (val_power = 1.0; isdigit(*(s + i)); i++) /* convert to the nth column */
    {
        val = 10.0 * val + (*(s + i) - '0');
        val_power *= 10.0;
    }
    if (*(s + i) == 'e' || *(s + i) == 'E') /* check for an exponent symbol */
    {
        exponent_sign = (*(s+ ++i) == '-') ? -1 : 1; /* store the exponent sign */
        if (*(s + i) == '-' || *(s + i) == '+')
        {
            s++;
        }
        for (exponent = 0.0; isdigit(*(s + i)); i++) /* convert to the nth column */
        {
            exponent = 10.0 * exponent + (*(s + i) - '0');
        }
        return power(sign * val / val_power, exponent_sign * exponent);
    }
    return sign * val / val_power; /* convert the number and add the sign */
}

/* x raised to the power of y.
Handles positive and negative whole numbered exponents */
double power(double x, int y)
{
    double result = 1.00;
    int i;
    if (y<0)
    {
        y=-1*y;
        x=1/x;
    }
    for (i=1;i<=y;i++)
    {
        result *= x;
    }
    return result;
}

void itoa(int n, char *s, unsigned minimum_field_width)
{
    int i = 0;
    int sign = (n < 0) ? -1 : 1;
    int digit_count = 0;

    do
    {
        *(s + i++) = sign * (n % 10) + '0';
        digit_count++;
    } while ((n /= 10) != 0);
    if (sign < 0)
    {
        *(s + i++) = '-';
        digit_count++;
    }
    *(s + i) = '\0';    
    reverse(s);

    /* Insert blanks */
    int number_of_blanks_to_insert = minimum_field_width - digit_count;
    if (number_of_blanks_to_insert > 0)
    {
        int s_length = strlen(s) - 1;
        char temp[MAX_SIZE];
        int i;
        for (i = 0; i < number_of_blanks_to_insert; ++i)
        {
            *(temp + i) = ' ';
        }
        *(temp + i) = '\0';

        strcat(temp, s); /* Append our number string to the end of the blanks */
        strcpy(s, temp); /* Copy the correctly formatted temp over our number string */
    }
}

/* From page 62.
Reverse string s in place. */
void reverse(char *s)
{
    int c, i, j;
    for(i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = *(s + i);
        *(s + i) = *(s + j);
        *(s + j) = c;
    }
}

int strrindex(char *s, char *t)
{
    int i, j, k;

    for(i = strlen(s) - 1; i >= 0; --i)
    {
        for(j = i, k = 0; *(t + k) != 0 && *(s + j)  == *(t + k); j++, k++)
            ;
        if (k > 0 && *(t + k)  == '\0')
        {
            return i;
        }
    }
    return -1;
}