/* Write the function htoi(s), which converts a string of hexadecimal
digits (including an optional 0x or 0X) into its equivalent integer value.
The allowable digits are 0 through 9, a through f, and A through F. */
#include <stdio.h>

#define BASE 16

int htoi(char hex[]);
int upper(int c);

int main()
{
    char fifteen[] = "0XF";
    char fourty_two[] = "2A";
    char hundred_thousand[] = "186A0";

    printf("%i\n", htoi(fifteen));
    printf("%i\n", htoi(fourty_two));
    printf("%i\n", htoi(hundred_thousand));

    return 0;
}

/* Convertx a hex string to an integer. Ignores invalid characters. */
int htoi(char hex[])
{
    int first = upper(hex[0]);
    int second = upper(hex[1]);
    int pos = 0;
    int num = 0;
    char hex_map[] = {"0123456789ABCDEF"};

    if (first == '0' && second == 'X') /* Optional 0X used? */
    {
        pos = 2;
    }

    /* Cycle each character of the input, then make sure it's a valid hex character. */
    for (int i = pos; hex[i] != '\0'; i++)
    {
        for (int j = 0; j < BASE; j++)
        {
            if (hex[i] == hex_map[j])
            {
                num = num * BASE + j;
                break;
            }
        }
    }

    return num;
}

/* Convert lower case ASCII to upper case ASCII */
int upper(int c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - 32;
    }
    else
    {
        return c;
    }
}