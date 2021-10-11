/* Write a program that converts upper case to lower or lower case to upper, depending on the name it is invoked with, as found in argv[0].

    Call the program with -l (ell) for lower case, -h for upper case. Default is lower case.
 */

#include <stdio.h>

void to_lower(void);
void to_upper(void);

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Error: too many arguments\n");
        return -1;
    }
    unsigned is_tolower = 0;


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