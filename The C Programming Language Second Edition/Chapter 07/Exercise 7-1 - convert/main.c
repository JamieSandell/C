/* Write a program that converts upper case to lower or lower case to upper, depending on the name it is invoked with, as found in argv[0].

    Call the program with -lower for lower case, -uppper for upper case. Default is lower case.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

const char lower[] = "-lower";
const char upper[] = "-upper";

void to_lower(void);
void to_upper(void);

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Error: too many arguments\n");
        return -1;
    }
    if (argc == 1)
    {
        printf("no argument specified, using the default: lower\n");
        to_lower();
        return 0;
    }
    if (argv[1][0] != '-')
    {
        printf("Error: arguments start with -\n");
        return -1;
    }
    

    if (strcmp(argv[1], lower) == 0)
    {
        to_lower();
    }
    else if(strcmp(argv[1], upper) == 0)
    {
        to_upper();
    }
    else
    {
        printf("Error: %s is an invalid argument\n", argv[1]);
    }

    return 0;
}

void to_lower(void)
{
    int c;
    while ((c = getchar()) != EOF)
    {
        putchar(tolower(c));
    }
}

void to_upper(void)
{
    int c;
    while((c = getchar()) != EOF)
    {
        putchar(toupper(c));
    }
}