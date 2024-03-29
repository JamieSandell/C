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
    unsigned int text   : 1;
} flags;

static const char hex[] = "-hex";
static const char oct[] = "-oct";

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Error: too many flags provided. -hex or -oct\n");
        return -1;
    }
    flags.hex = 0;
    flags.oct = 0;
    flags.text = 0;
    if (argc == 1)
    {
        flags.hex = 1;
    }
    else
    {
        if (strcmp(argv[1], hex) == 0)
        {
            flags.hex = 1;
        }
        else if (strcmp(argv[1], oct) == 0)
        {
            flags.oct = 1;
        }
        else
        {
            printf("Error: %s is not a valid flag, use -hex or -oct\n", argv[1]);
            return -1;
        }
    }

    int c;
    unsigned column = 0;
    while ((c = getchar()) != EOF)
    {
        if (!isprint(c)) /* non-graphic character  */
        {
            if (flags.text)
            {
                flags.text = 0;
                printf("\nPrinting non-text\n");
                column = 0;
            }
            if ((column + 4) > COL_SIZE) /* keep within the column width set */
            {
                printf("\n");
                column = 0;
            }
            if (flags.hex)
            {
                printf("%#.2x", c);
            }
            else
            {
                printf("%.3o", c);
            }            
            column += 4;
        }
        else
        {
            if (!flags.text)
            {
                flags.text = 1;
                printf("\nPrinting text\n");
                column = 0;
            }
            switch (c)
            {
                case '\n':
                    printf("\n");
                    column = 0;
                    break;
                case '\t':
                    if ((column + TAB_SIZE) > COL_SIZE)
                    {
                        printf("\n");
                        column = 0;
                    }
                    printf("\t");
                    column += TAB_SIZE;
                    break;
                default:
                    if ((column + 1) > COL_SIZE)
                    {
                        printf("\n");
                        column = 0;
                    }
                    printf("%c", c);
                    column += 1;
                    break;
            }
        }
    }

    return 0;
}