/* Write a program that will print arbitrary input in a sensible way.
As a minimum, it should print non-graphic characters in octal or hexadecimal
according to local custom

Run with -hex or -oct.

-hex is the default
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define COL_SIZE 40 /* how many characters wide can the output be before a newline */
#define TAB_SIZE 8 /* how many characters wide a tab is */

struct
{
    unsigned int hex    : 1;
    unsigned int oct    : 1;
} flags;

static const char hex[] = "-hex";

int main(int argc, char *argv[])
{
    int c;
    if (argc > 2)
    {
        printf("Error: too many flags provided. -hex or -oct\n");
        return -1;
    }
    if (argc == 1)
    {
        flags.oct = 0;
        flags.hex = 1;
    }
    else
    {
        /*if (strcmp())*/
    }


    while ((c = getchar()) != EOF)
    {
        if (!iscntrl(c) && !isprint(c)) /* non-graphic character  */
        {
        }
    }

    return 0;
}