/* Extend atof to handle scientific notation of the form
    123.45e-6
where a floating-point number may be followed by e or E and an optionally
signed exponent.

atof from page 71:
#include <ctype.h>

# atof: convert string s to double
double atof(char s[])
{
    double val, power;
    int i, sign;

    for (i = 0; isspace(s[i])); i++)
        ; # skip white space
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-')
    {
        i++;
    }
    for (val = 0.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + (s[i] - '0');
    }
    if (s[i] == '.')
    {
        i++;
    }
    for (power = 1.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }
    return sign * val / power
} */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

double atof(char s[]);

int main()
{

}

double atof(char s[])
{
    double val, power;
    int i, sign;

    for (i = 0; isspace(s[i])); i++)
        ;
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-')
    {
        i++;
    }
    for (val = 0.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + (s[i] - '0');
    }
    if (s[i] == '.')
    {
        i++;
    }
    for (power = 1.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }
    return sign * val / power
}