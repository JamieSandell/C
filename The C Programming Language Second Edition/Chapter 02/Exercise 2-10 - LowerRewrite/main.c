/* Rewrite the function lower, which converts upper case letters to lower case, with a conditional expression instead of if-else.
int lower(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + 'a' - 'A';
    }
    else
    {
        return c;
    }
}
 */

#include <stdio.h>

int lower(int c);

int main()
{
    char name[] = {"JAMIE"};
    unsigned size = 5;

    printf("%s converted to lower case is : ", name);
    for (int i = 0; i < size; i++)
    {
        putchar(lower(name[i]));
    }
    putchar('\n');

    return 0;
}

int lower(int c)
{
    return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}
