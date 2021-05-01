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

    for (i = 0; isspace(s[i]); i++)
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
double power(double x, double y);

int main()
{
    char number[] = {"10e2"};
    printf("%s converted to double: %f\n", number, atof(number));
}

double atof(char s[])
{
    double val, val_power, exponent, exponent_power;
    int i, sign, exponent_sign;

    for (i = 0; isspace(s[i]); i++)
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
    for (val_power = 1.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + (s[i] - '0');
        val_power *= 10.0;
    }
    if (s[i] == 'e' || s[i] == 'E')
    {
        exponent_sign = (s[++i] == '-') ? -1 : 1;
        if (s[i] == '-' || s[i] == '+')
        {
            s++;
        }
        for (exponent = 0.0; isdigit(s[i]); i++)
        {
            exponent = 10.0 * exponent + (s[i] - '0');
        }
        if (s[i] == '.')
        {
            i++;
        }
        for (exponent_power = 1.0; isdigit(s[i]); i++)
        {
            exponent = 10.0 * exponent + (s[i] - '0');
            exponent_power *= 10.0;
        }
        return power(sign * val / val_power, exponent_sign * exponent / exponent_power);
    }
    return sign * val / val_power;
}

double power(double x, double y)
{
    double result = x;
    int i;

    if (y)
    {
        for (i = 0; i < y - 1; i++)
        {
            result *= x;
        }
        return result;
    }    

    return result;
}